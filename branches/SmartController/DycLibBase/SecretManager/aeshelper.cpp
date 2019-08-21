#include "aeshelper.h"

AesHelper::AesHelper()
{
  unsigned char key[] = "1p*2w+3e&4r";
  aes = new AES(key);
}

AesHelper::~AesHelper()
{
  delete aes;
  aes = NULL;
}

QString AesHelper::aesEncrypt(QString mingwen)
{
  QString result = QString("");
  char ch_mingwen[10240];
  char ch_miwen[10240];
  strcpy(ch_mingwen, mingwen.toUtf8().data());
  aes->Cipher(ch_mingwen, ch_miwen);
  result = QString(ch_miwen);
  return result;
}

QString AesHelper::aesUncrypt(QString miwen)
{
  QString result = QString("");
  char ch_mingwen[10240];
  char ch_miwen[10240];
  strcpy(ch_miwen, miwen.toUtf8().data());
  aes->InvCipher(ch_miwen, ch_mingwen);
  result = QString(ch_mingwen);

  return result;
}
