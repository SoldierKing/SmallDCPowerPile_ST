#include "ADrv_KeyDown.h"


/// <summary>
///  ��ȡ���������µ�ʱ��
/// </summary>
/// <param name="StructKeyPressCfg *pKeyPressCfg">��������ָ��</param>  
void GetKeyKeepDownTime(StructKeyPressCfg *pKeyPressCfg)
{

  if (pKeyPressCfg->IOSta == pKeyPressCfg->IsDown)
  {
    if (pKeyPressCfg->BtnTick == 0)
      pKeyPressCfg->BtnTick = GetSystemTick();
    else
      pKeyPressCfg->KeyDownTime = GetSystemTick() - pKeyPressCfg->BtnTick;
  }
  else if (pKeyPressCfg->IOSta  == pKeyPressCfg->IsUp)
  {
    
    if (pKeyPressCfg->BtnTick != 0) 
      pKeyPressCfg->KeyDownTime = GetSystemTick() - pKeyPressCfg->BtnTick;
    pKeyPressCfg->BtnTick = 0;
  }
  
}

/// <summary>
///  ��ȡ�����ǳ������Ƕ̰�
/// </summary>
/// <param name="StructKeyPressCfg *pKeyPressCfg">��������ָ��</param>  
EnumKeyType GetKeyPressType(StructKeyPressCfg *pKeyPressCfg)
{
  
  GetKeyKeepDownTime(pKeyPressCfg);
  if(pKeyPressCfg->KeyDownTime > pKeyPressCfg->KeyLongPressTime
     && pKeyPressCfg->IOSta == pKeyPressCfg->IsUp)
  {
    pKeyPressCfg->KeyDownTime = 0;
    return Enum_KeyLongPress;
  }
  else if (pKeyPressCfg->KeyDownTime > pKeyPressCfg->KeyShortPressTime
            && (pKeyPressCfg->IOSta == pKeyPressCfg->IsUp))
  {
    pKeyPressCfg->KeyDownTime = 0;
    return Enum_KeyShortPress;
  }
  
  //pKeyPressCfg->KeyDownTime = 0;
  if (pKeyPressCfg->IOSta == pKeyPressCfg->IsDown)
    return Enum_KeyDown;
  else 
    return Enum_KeyUp;
}




