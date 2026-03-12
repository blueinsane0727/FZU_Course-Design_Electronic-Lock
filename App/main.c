#include <REGX52.H>
#include "Delay.h"
#include "MatrixKey.h"
#include "Nixie.h"
#include "Motor.h"
#include "beep.h"
#include "24c02.h"

// EEPROM地址定义
#define EEPROM_PASSWORD_ADDR_HIGH  0x00  // 密码高8位存储地址
#define EEPROM_PASSWORD_ADDR_LOW   0x01  // 密码低8位存储地址
#define EEPROM_INIT_FLAG_ADDR      0x02  // 初始化标志存储地址
#define EEPROM_INIT_FLAG_VALUE     0x5A  // 初始化标志值

unsigned char KeyNum;
unsigned char False_Password;	//错误密码次数
unsigned int Password,Count,True_Password;
unsigned char DisplayBuffer[4];	//数码管显示缓冲区
unsigned char Unlocked;	//标志已经开锁
unsigned char ChangingPassword;  // 修改密码标志：0=正常模式，1=修改密码模式
unsigned int NewPassword = 0;        // 新密码临时存储
unsigned char NewPasswordCount = 0;  // 新密码输入计数

void NormalMode(unsigned char key);
void ChangePasswordMode(unsigned char key);
void CheckEEPROMInit(void);
void SavePasswordToEEPROM(unsigned int password);
unsigned int LoadPasswordFromEEPROM(void);

void main()
{
	CheckEEPROMInit();
	True_Password = LoadPasswordFromEEPROM();
	//True_Password = 111;
	False_Password = 0;
	Unlocked = 0;
	ChangingPassword = 0;
	
	DisplayBuffer[0] = 0;
    DisplayBuffer[1] = 0;
    DisplayBuffer[2] = 0;
	
	while(1)
	{
		
		
		Nixie(1, DisplayBuffer[0]);
        Nixie(2, DisplayBuffer[1]);
        Nixie(3, DisplayBuffer[2]);
		
		KeyNum=MatrixKey();
		if(KeyNum)
		{
			
			if(ChangingPassword)
            {
                ChangePasswordMode(KeyNum);
            }
            else
            {
                NormalMode(KeyNum);
            }
		}
	}
}

/**
  * @brief  正常模式，按下s1-s10输入密码，按下s11确认密码，按下s12清空输入密码，按下s13回退一位，按下s14修改密码（只在开锁后有效）
  * @param  矩阵键盘扫描的键值
  * @retval 无
  */
void NormalMode(unsigned char key)
{
	if(key <= 10)  //如果按下S1-S10，输入密码
	{
		if(Count<3)  
		{
			Password*=10;  //密码左移一位
			Password+=key%10;  //获取一位密码
				
			DisplayBuffer[Count] = 10;
			Count++;  //计次加一
		}
	}
	if(key == 11)	//将S11定义为确认键
	{
		if(Password == True_Password)	//判断输入密码是否正确
		{
			Unlocked = 1; 
			Password=0;	//密码清零
			Count=0;	//计次清零
			DisplayBuffer[0] = 0;
            DisplayBuffer[1] = 0;
            DisplayBuffer[2] = 0;
			False_Password = 0;
			Open();

		}else	//若密码错误
		{
			False_Password++;
			Password=0;	//密码清零
			Count=0;	//计次清零
			DisplayBuffer[0] = 0;
            DisplayBuffer[1] = 0;
            DisplayBuffer[2] = 0;
			display_false(False_Password);
			if(False_Password >= 5)
			{
				BeepWorking();
			}

		}
	}
	if(key == 12)	//将s12定义为取消键
	{
		Password=0;	//密码清零
		Count=0;	//计次清零
		DisplayBuffer[0] = 0;
		DisplayBuffer[1] = 0;
		DisplayBuffer[2] = 0;

	}
	if(key == 13)	//将s13定义为删除键
	{
		if(Count > 0)
		{
			Count--;
			Password /= 10;
			DisplayBuffer[Count] = 0;
		}

	}
	if(key == 14)	//将s14定义为修改密码键
	{
		if(Unlocked)
		{
			ChangingPassword = 1;
			DisplayBuffer[0] = 10;
            DisplayBuffer[1] = 10;
            DisplayBuffer[2] = 10;
		}

	}
	if(key == 15)	//将s15定义为关锁键
	{
		if(Unlocked)
		{
			Unlocked = 0; 
			Close();
		}
	}
}

/**
  * @brief  当在开锁后按下s14后切换到修改密码模式，在此模式下，按下s1-s10，显示按下的新密码，s11确认新密码，s12取消，s13回退一位
  * @param  矩阵键盘的按键键码
  * @retval 无
  */
void ChangePasswordMode(unsigned char key)
{
	unsigned char digit;
	if(key <= 10)  //如果按下S1-S10，输入密码
	{
		if(NewPasswordCount<3)  
		{
			digit = key % 10;
			NewPassword*=10;  //密码左移一位
			NewPassword+=digit%10;  //获取一位密码
				
			DisplayBuffer[NewPasswordCount] = digit;
			NewPasswordCount++;  //计次加一
		}
	}
	if(key == 11)	//将S11定义为确认键
	{
		True_Password = NewPassword;
		SavePasswordToEEPROM(True_Password);
		ChangingPassword = 0;
        Unlocked = 0; 
		NewPassword = 0;
		NewPasswordCount = 0;
		Close();
		DisplayBuffer[0] = 0;
		DisplayBuffer[1] = 0;
		DisplayBuffer[2] = 0;
	}
	if(key == 12)	//将s12定义为取消键
	{
		NewPassword=0;	//密码清零
		NewPasswordCount=0;	//计次清零
		DisplayBuffer[0] = 10;
        DisplayBuffer[1] = 10;
        DisplayBuffer[2] = 10;

	}
	if(key == 13)	//将s13定义为删除键
	{
		if(NewPasswordCount > 0)
		{
			NewPasswordCount--;
			NewPassword /= 10;
			DisplayBuffer[NewPasswordCount] = 10;
		}

	}
}

/**
  * @brief  检查EEPROM是否已初始化，如果没有则写入默认值
  * @param  无
  * @retval 无
  */
void CheckEEPROMInit(void)
{
	unsigned char init_flag = at24c02_read_one_byte(EEPROM_INIT_FLAG_ADDR);
	
	if(init_flag != EEPROM_INIT_FLAG_VALUE)
	{
		SavePasswordToEEPROM(111);
		at24c02_write_one_byte(EEPROM_INIT_FLAG_ADDR, EEPROM_INIT_FLAG_VALUE);
		Delay(10);
	}
}

/**
  * @brief  保存密码到EEPROM
  * @param  password: 要保存的密码（0-999）
  * @retval 无
  */
void SavePasswordToEEPROM(unsigned int password)
{
	// 将16位密码拆分为两个8位字节
	unsigned char high_byte = (password >> 8) & 0xFF; 
	unsigned char low_byte = password & 0xFF;         
	
	// 写入EEPROM
	at24c02_write_one_byte(EEPROM_PASSWORD_ADDR_HIGH, high_byte);
	Delay(5);  
	at24c02_write_one_byte(EEPROM_PASSWORD_ADDR_LOW, low_byte);
	Delay(5);
}

/**
  * @brief  从EEPROM加载密码
  * @param  无
  * @retval 读取的密码
  */
unsigned int LoadPasswordFromEEPROM(void)
{
	// 从EEPROM读取两个字节
	unsigned char high_byte = at24c02_read_one_byte(EEPROM_PASSWORD_ADDR_HIGH);
	unsigned char low_byte = at24c02_read_one_byte(EEPROM_PASSWORD_ADDR_LOW);
	
	unsigned int password = (high_byte << 8) | low_byte;
	
	if(password > 999)
	{
		password = 111;
		SavePasswordToEEPROM(password);  
	}
	
	return password;
}

