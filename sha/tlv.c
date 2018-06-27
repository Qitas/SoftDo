#include "tlv.h"

/* 
 * ������ݱ��ĵĳ��ȣ�����ͷ��ENDTLV
 * */
u8 getkDatagramLen(DATA_TLV_S* firstTlvData, u8*dataLen)
{
    DATA_TLV_S* tlvData = firstTlvData;
    u16 length = 0;
    
    if ((NULL == tlvData) || (NULL == dataLen))
    {
        return TLV_ERROR;
    }
    
    while(NULL != tlvData)
    {
        if (TLV_SUCCESS != isTlvValid(tlvData))
        {
            return TLV_ERROR;
        }
        
        length += 1; //TYPE��
        if (TLV_DATA_TYPE_END == tlvData->type)
        {
            break;
        }
        
        length += 1;//������
        length += tlvData->len; 
        
        //���Ȳ���Խ��
        if (length > TVL_DATA_MAX_LEN)
        {
            return TLV_ERROR;
        }
        
        tlvData = getNextTlv(tlvData);
    }
    
    *dataLen = (u8)length;
    
    return TLV_SUCCESS;
}

/* 
 * ����������ݱ����Ƿ�������CHECKУ��,��������TLV_SUCCESS
 * */
u8 checkDatagramValid(DATA_TLV_S* firstTlvData)
{
    u8 dataCheckSum = 0;
    u8 calcCheckSum = 0;
    
    if (NULL == firstTlvData)
    {
        return TLV_ERROR;
    }
    
    //��һ����checksum
    if (firstTlvData->type != TLV_DATA_TYPE_CHECKSUM)
    {
        return TLV_ERROR;
    }
    
    //checksum�ĳ�����
    if (firstTlvData->len != TLV_CHECKSUM_VALUE_LEN)
    {
        return TLV_ERROR;
    }
    
    dataCheckSum = *(firstTlvData->value);
    
    //���㵽��checksum�ͱ�����ĶԱ�
    if (TLV_SUCCESS != computCheckSum(firstTlvData,&calcCheckSum))
    {
        return TLV_ERROR;
    }
    
    return (calcCheckSum == dataCheckSum) ? TLV_SUCCESS : TLV_ERROR;
}

/* 
 * ��鵱ǰTLV�Ƿ�������CHECKУ��ͨ������TLV_SUCCESS
 * */
u8 isTlvValid(DATA_TLV_S*tlvData)
{
    if (NULL == tlvData)
    {
        return TLV_ERROR;
    }
    
    if (TLV_DATA_TYPE_END == tlvData->type)
    {
        return TLV_SUCCESS;
    }
    
    //���ݳ��Ȳ���Խ��
    if ((0 == tlvData->len) || (tlvData->len > (TVL_DATA_MAX_LEN-2-3))) //-�����ݵ�TYPELEN��STARTTLV�ĳ���
    {
        return TLV_ERROR;
    }
    
    return TLV_SUCCESS;
}

/* 
 * ����CHECKSUM��������STARTTLV������ENDTLV
 * ����ɹ�����TLV_SUCCESS
 * */
u8 computCheckSum(DATA_TLV_S* firstTlvData, u8* checkSum)
{
    DATA_TLV_S* tlvData = firstTlvData;
    u8 i;

    if (NULL == checkSum)
    {
        return TLV_ERROR;
    }
    
		*checkSum = 0;
    //��START��ĵ�һ������TLV��ʼ
    while (NULL != (tlvData = getNextTlv(tlvData)))
    {
        if (tlvData - firstTlvData > TVL_DATA_MAX_LEN)
        {
           return TLV_ERROR; 
        }
        
        if (TLV_DATA_TYPE_END == tlvData->type)
        {
            break;
        }
        
        if (!isTlvValid(tlvData))
        {
            return TLV_ERROR;
        }
        
        //����У�������ķ�ʽ
        *checkSum ^= tlvData->type;
        *checkSum ^= tlvData->len;
        
        for (i =0; i < tlvData->len; ++i)
        {
            *checkSum ^= tlvData->value[i];
        }
    }
    
    return TLV_SUCCESS;
}

/* 
 * ��õ�һ��TLV���������ݱ���ԭ��
 * */
DATA_TLV_S* getFirstTlv(u8 *data)
{
    if (NULL == data)
    {
        return NULL;
    }
    
    return (DATA_TLV_S*)data;
}

/* 
 * ͨ����TLV��ѯ��һ��TLV������
 * ���ؿձ�ʾû����һ����
 * */
DATA_TLV_S* getNextTlv(DATA_TLV_S* tlvData)
{
    u8 *data = NULL;

    if (TLV_SUCCESS != isTlvValid(tlvData))
    {
        return NULL;
    }
    
    if (TLV_DATA_TYPE_END == tlvData->type)
    {
        return NULL;
    }
    
    data = (u8*)tlvData;

    //��λ��VALUE��
    data += 2;
    
    //��λ�����ݽ�β
    data += tlvData->len;

    return (DATA_TLV_S*)data;
}

//����ʵ��д��ĳ���
int toString(DATA_TLV_S* tlvData, char* buffer, int bufferLen)
{
    int dataLen = 0;
    int i;

    if (NULL == tlvData || NULL == buffer || 0 == bufferLen)
    {
        return 0;
    }
		
		if (tlvData->type == TLV_DATA_TYPE_END)
		{
		    return 0;
		}

    dataLen = snprintf(buffer,bufferLen,"T:%02dL:%02d:V:",tlvData->type,tlvData->len);

    for (i = 0; (i < tlvData->len) && (dataLen < bufferLen); ++i)
    {
        dataLen += snprintf(buffer + dataLen, bufferLen - dataLen, "%02X", tlvData->value[i]);
    }
		
		//�ӻس���
		if ((i > 0) && (dataLen < bufferLen))
		{
		    dataLen += snprintf(buffer + dataLen, bufferLen - dataLen, "\n");
		}

    return dataLen;
}


