#ifndef T_LOG_TEMPLATE_H
#define T_LOG_TEMPLATE_H


#include "singleton.h"

typedef struct{
    int templateType;
    int canShutDown;
    int status;
    QString templateContent;
} NoteTemplateDesc_t;

/*
typedef struct{
    noteInfo takeCodeInfo; //1 收件人取件模板
    noteInfo takePackInfo;   //2 用户取出派件模板
    noteInfo expTakePackInfo;  //3 快递员取出派件
    noteInfo senderInfo;  //11 寄件通知寄件人模板
    noteInfo senderExpInfo;    //12 寄件通知揽件人
    noteInfo senderTakeInfo;     //13寄件被取走，再次通知寄件人模板
    noteInfo storageInfo;     //21 临时寄存存入模板。
    noteInfo forgetCodeInfo;   //91 忘记取件码
    noteInfo takeCellErInfo;  //92 格子异常，通知取件人模板
    noteInfo reportCellErInfo;  //93 格子异常，通知维修模板
    noteInfo repairtCellErInfo;  // //94 格子异常，修复通知取件人模板
} NoteTemplateDesc_t;
*/

class CNoteTemplate
{
public:
    QList<NoteTemplateDesc_t> noteTemplateInfo;
public:
    CNoteTemplate();

    void init();

    void getNoteTemplateInfo();

    bool setNoteTemplateInfo();

    bool getCurrentNoteInfo(int noteType,QString &content);

    QString getSaveNoteInfo(QString info,QString exp,QString terminal,QString code,QString time,QString cellId,QString inform);

    bool setTemplateId(QString templateId);
    bool setTemplateName(QString templateName);
    bool setAddresseeContent(QString addresseeContent);
    bool setGetPasswordContent(QString getPasswordContent);
    bool setCellErrorContent(QString cellErrorContent);
    bool setrepairContent(QString repairContent);
    bool setNotifyReairContenxt(QString notifyReairContenxt);
    bool setExpGetDeliveryContent(QString expGetDeliveryContent);
    bool setSenderSaveContent(QString senderSaveContent);
    bool setLanJianContent(QString lanJianContent);

private:
    QTextCodec *tc;
    DECLARE_SINGLETON_CLASS(CNoteTemplate)
};

typedef Singleton<CNoteTemplate> SCNoteTemplate;

#endif // T_LOG_TEMPLATE_H
