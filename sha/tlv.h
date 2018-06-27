#ifndef __DATA_GRAM_H__
#define __DATA_GRAM_H__

#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"

#define TLV_DATA_TYPE_END     		  0
#define TLV_DATA_TYPE_CHECKSUM		  1

#define TLV_CHECKSUM_VALUE_LEN 	  	1
#define TVL_DATA_MAX_LEN       	  250

#define TLV_SUCCESS 1
#define TLV_ERROR   0

/*
Լ����
1����һ��TLV�̶���CHECKSUM��type=TLV_DATA_TYPE_CHECKSUM,len=1,value=1���ֽڵ�checkֵ
2���������ĵĳ��Ȳ��ܳ���TVL_DATA_MAX_LEN���ֽڣ�����ͷ��β
3���������ͳ�TLV_DATA_TYPE_END�⣬�������͵�len��������0
*/

typedef struct DATA_TLV_S_ 
{
    u8 type;
    u8 len;
    u8 value[1];
}DATA_TLV_S;

/* 
 * ����������ݱ����Ƿ�������CHECKУ��,��������TLV_SUCCESS
 * */
u8 checkDatagramValid(DATA_TLV_S* firstTlvData);

/* 
 * ��õ�һ��TLV���������ݱ���ԭ��
 * */
DATA_TLV_S* getFirstTlv(u8 *data);

/* 
 * ͨ����TLV��ѯ��һ��TLV������
 * ���ؿձ�ʾû����һ����
 * */
DATA_TLV_S* getNextTlv(DATA_TLV_S* tlvData);

/* 
 * ��鵱ǰTLV�Ƿ�������CHECKУ��ͨ������TLV_SUCCESS
 * */
u8 isTlvValid(DATA_TLV_S*tlvData);

/* 
 * ����CHECKSUM��������STARTTLV������ENDTLV
 * ����ɹ�����TLV_SUCCESS
 * */
u8 computCheckSum(DATA_TLV_S* firstTlvData, u8* checkSum);

/* 
 * ������ݱ��ĵĳ��ȣ�����ͷ��ENDTLV
 * */
u8 getkDatagramLen(DATA_TLV_S* firstTlvData, u8*dataLen);


//����ʵ��д��ĳ���
int toString(DATA_TLV_S* tlvData, char* buffer, int bufferLen);

#endif
