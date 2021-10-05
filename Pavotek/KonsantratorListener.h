#ifndef AKILLIETIKETMAIN_H
#define AKILLIETIKETMAIN_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <../mqtt_files/qmqtt.h>
#include <QTimer>
#include <QSettings>

#include <qnetworkaccessmanager.h>
#include <QNetworkRequest>
#include <qnetworkconfigmanager.h>
#include <QNetworkReply>
#include <QFile>
#include <QProcess>

#define MS  1       //Milisaniye
#define SN  1000    //Saniye
#define DK  SN*60   //Dakika
#define SA  DK*60   //Saat

#define defMsgTimeout 3*SN

class KonsantratorListener : public QObject
{
    Q_OBJECT

public:
    explicit KonsantratorListener(QStringList arg, QObject *parent = 0);
    ~KonsantratorListener();

    enum enAdvType{
        IMAJ,
        DURUM,
        KURULUM,
        UNKNOWN
    };

    typedef enum{
                fromMABdelOneAdv           =   0xA0,
                fromMABupdateData          =   0xA1,
                fromMABstartAdv            =   0xA2,
                fromMABresetBLE            =   0xA3,
                fromMABdummy               =   0xA5,
                fromMABswitchToOTA         =   0xA7,
                fromMABeslestirmeData      =   0xAB,
                fromMABswitchToEslestirme  =   0xAC,
                fromMABgetVersion          =   0xB9,
                fromMABdelAllAdv           =   0xE2,
                fromMABgetCurrentAdv       =   0xE3,
                fromMABdisconnectDevice    =   0xE5
                                           }eFromMABmessageTypes;

    typedef enum {
                dataForImage               =   0xD4,
                dataForStates              =   0xD6,
                dataForEslestirme          =   0xD8
                                           }eDataRequestTypes;

    typedef enum{
                fromESPtalkServer          =   0xA4
                                           }eFromESPmessageTypes;


    typedef enum{
                toMABeslestirmeIstegi               =   0xD1,
                toMABdataRequest                    =   0xD3,
                toMABdataUpdated                    =   0xD5,
                toMABtimeoutNack                    =   0xD7,
                toMABeslestirmeACK                  =   0xD9,
                toMABeslestirmeStarted              =   0xDA,

                toMABmessageNACKfromEtiket          =   0xE1,
                toMABdeletedAllAdv                  =   0xE2,
                toMABsendCurrentAdv                 =   0xE3,

                toMABdeletedOneAdv                  =   0xA0,
                toMABmessageACK                     =   0xA5,
                toMABmessageNACK                    =   0xA6,
                toMABmessageSenddedToRemoteDevice   =   0xA9,
                toMABstartedAdv                     =   0xAA,

                toMABrestartedServerBLE             =   0xB0,
                toMABconnectedDevice                =   0xB1,
                //pasif                             =   0xB2,
                toMABswitchedToOTA                  =   0xB8,
                toMABsendVersion                    =   0xB9,
                toMABdisconnecttedDevice            =   0xBB
                                                    }eToMABmessageTypes;

    typedef struct{
        quint8 msgType;
        quint8 advID;
        quint8 pageID;

    }tsResponseMessageFormat;

    typedef struct{
        quint8 msgType;
        quint8 len_msb;
        quint8 len_lsb;
        quint8 advID;
        quint8 pageID;
        quint8 byteNum;
        quint8 bitNum;
    }tsGuncellemeFormat;

    typedef struct{
        quint8 msgType;
        quint8 len_msb;
        quint8 len_lsb;
        quint8 advID;
        quint8 pageID;
    }tsStandartFormat;

    QString konsTopic = "akilliEtiket/Kons/+/rq";

    QString serverIP  ;
    QString mqttClientID;
    QString mqttUser;
    QString mqttPass;
    QString dbUser ;
    QString dbPass ;
    QString dbName;
    QString KonsantratorID;

    QByteArray _tempMsg;
    QByteArray _tempData;

    uint8_t IsChBusy[4]={0,0,0,0,};

public slots:
    void postData(QByteArray sendData, QString Path);
    void writeDebug(QString message);
    void getResponse(QNetworkReply* reply);
    //    void sendMSG(QString topic, QByteArray data);
    void connected();
    void disconnected();
    void onReceived(const QMQTT::Message& message);
    void subscribed(QString topic);
private slots:
    void clearTempMsg(){         _tempMsg.clear(); _tempData.clear();    }

private:
    QMQTT::Client *client;
    QTimer *msgTimeout;
};

#endif // AKILLIETIKETMAIN_H
