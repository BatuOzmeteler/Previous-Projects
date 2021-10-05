#include "KonsantratorListener.h"

KonsantratorListener::KonsantratorListener(QStringList arg, QObject *parent)
    : QObject(parent)
{
    KonsantratorID = arg.at(1);
    writeDebug(tr("Program Opened! %1").arg( QString(arg.at(0)).split("_")[1] ));

    if ( KonsantratorID.isEmpty() || KonsantratorID.isNull()) return;
    client = new QMQTT::Client("localhost",1883,false,false);

    client->setAutoReconnect(true);
    client->setUsername("akilliEtiket");
    client->setPassword("pavo");


    client->setClientId(tr("KonsantratorListener_%1").arg(KonsantratorID));

    konsTopic = konsTopic.replace("+",KonsantratorID);

    connect(client,SIGNAL(connected()),this,SLOT(connected()));
    connect(client,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(client,SIGNAL(received(QMQTT::Message)),this,SLOT(onReceived(QMQTT::Message)));
    connect(client,SIGNAL(subscribed(QString)),this,SLOT(subscribed(QString)));

    client->connectToHost();

    msgTimeout = new QTimer(this);
    msgTimeout->setInterval(0);
    msgTimeout->stop();
    connect(msgTimeout,SIGNAL(timeout()),this,SLOT(clearTempMsg()));

}

void KonsantratorListener::subscribed(QString topic){
    postData("",tr("BLEProcess.php?a=%1&t=reset").arg(KonsantratorID));
    writeDebug(tr("subscribed %1").arg(topic));
}

KonsantratorListener::~KonsantratorListener()
{

    client->unsubscribe(konsTopic);

    if(client->isConnectedToHost()) client->disconnectFromHost();
}


void KonsantratorListener::connected(){
    client->unsubscribe(konsTopic);
    writeDebug(tr("Connected to broker! %1").arg(client->clientId()));
    client->subscribe(konsTopic,0);
}

void KonsantratorListener::disconnected(){
    client->unsubscribe(konsTopic);
    writeDebug("Disconnected!");
}



void KonsantratorListener::onReceived(const QMQTT::Message& message){

    QString topic = message.topic();
    QString tempWrite;

    QString topicID = topic.split('/')[2];

    QByteArray _raw = message.payload();


    QByteArray data;
    for(int k=0;k<_raw.size();k++)
        tempWrite.append(tr("%1,").arg((quint8)_raw.at(k),0,16));

    qDebug()<<"";

    writeDebug(tr("Raw (%1): %2").arg(topic).arg(tempWrite));

    if ( _raw == _tempMsg){
        writeDebug("Tekrarlanan Mesaj");
        _tempMsg.clear();
        return;
    }

    msgTimeout->stop();

    msgTimeout->start(defMsgTimeout);
    _tempMsg = _raw;

    int startIndex = 0;

    if(_raw.indexOf(0x01) >= 0) {

        for(startIndex = _raw.indexOf(0x01); startIndex>= 0&& startIndex + 3 < _raw.size(); startIndex = _raw.indexOf(0x01,startIndex)){

            int len = 0;
            len = 1 + 1 + 1 + 1 + (_raw.at(startIndex + 2) * 256) + _raw.at(startIndex + 3) + 1 + 1;

            if(startIndex + len -1 >= _raw.size()) {
                startIndex++;
                continue;
            }

            if ( _raw.at(startIndex + len - 1) != 0x04){
                startIndex++;
                writeDebug("EOT not found!");
                continue;
            }

            data = _raw.mid(startIndex,len);

            data.remove(0,1);
            data.remove(data.size() - 1, 1);

            startIndex+= (len - 1);


            if ( _tempData == data ){
                writeDebug("Cakisma");
                continue;
            }

            quint8 msgType = data.at(0);

            quint8 reklamID = 0;
            quint8 pageID = 0;
      
            reklamID = data.at(3);
            pageID = data.at(4);

            if ( reklamID == 0 || pageID > 5 ){
                reklamID = topicID.toUInt();
                pageID = 0;
            }
            

            if ( msgType != toMABmessageACK )             _tempData = data;
            if ( msgType != toMABeslestirmeStarted && msgType != toMABeslestirmeIstegi && msgType != toMABeslestirmeACK && tr("%1").arg(reklamID) != topicID)
            {
                writeDebug(tr("ReklamID(%1) - TopicID(%2) uyumsuzlugu: %3").arg(tr("%1").arg(reklamID)).arg(topicID).arg(tr("%1").arg(reklamID) == topicID));
                postData(data,tr("test.php?t=reset&a=%1&p=%2").arg(topicID).arg(pageID));
            }

            switch (msgType) {

            case toMABdataUpdated                   : writeDebug(tr("(%1-%2) UpdateBasarili"              ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABtimeoutNack                   : writeDebug(tr("(%1-%2) TimeoutNACK"                 ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABeslestirmeACK                 : writeDebug(tr("(%1-%2) EslestirmeBasarili"          ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABeslestirmeStarted             : writeDebug(tr("(%1-%2) EslestirmeBasladi"           ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABmessageNACKfromEtiket         : writeDebug(tr("(%1-%2) HataliMesaj_Etiket"          ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABdeletedAllAdv                 : writeDebug(tr("(%1-%2) TumReklamlarSilindi"         ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABsendCurrentAdv                : writeDebug(tr("(%1-%2) GuncelReklam"                ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABdeletedOneAdv                 : writeDebug(tr("(%1-%2) TekReklamSilindi"            ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABmessageACK                    : writeDebug(tr("(%1-%2) MesajBasarili"               ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABmessageNACK                   : writeDebug(tr("(%1-%2) MesajBasarisiz"              ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABmessageSenddedToRemoteDevice  : writeDebug(tr("(%1-%2) messageSenddedToRemoteDevice").arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABstartedAdv                    : writeDebug(tr("(%1-%2) MABstartedAdv"               ).arg(reklamID).arg(pageID) ); postData(data,"response.php");IsChBusy[pageID] = 0;                                                     break;
            case toMABrestartedServerBLE            : writeDebug(tr("(%1-%2) restartedServerBLE"          ).arg(reklamID).arg(pageID) ); postData(data,tr("BLELife.php?reklamID=%1&pageID=%2").arg(reklamID).arg(pageID));  break;
            case toMABconnectedDevice               : writeDebug(tr("(%1-%2) connectedDevice"             ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABswitchedToOTA                 : writeDebug(tr("(%1-%2) switchedToOTA"               ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABsendVersion                   : writeDebug(tr("(%1-%2) sendVersion"                 ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABdisconnecttedDevice           : writeDebug(tr("(%1-%2) disconnecttedDevice"         ).arg(reklamID).arg(pageID) ); postData(data,"response.php");                                                     break;
            case toMABeslestirmeIstegi              : writeDebug(tr("(%1-%2) EslestirmeIstegi"            ).arg(reklamID).arg(pageID) ); postData(data, tr("eslestir.php?id=%1").arg(topicID));IsChBusy[pageID] = 0;                            break;

            case toMABdataRequest                   :{writeDebug(tr("(%1-%2-%3-%4) UpdateIstegi"                ).arg(reklamID).arg(pageID).arg(QString("%1").arg((quint8) data.at(5))).arg(QString("%1").arg((quint8) data.at(6))) );
                if ( IsChBusy[pageID] == 0){
                    IsChBusy[pageID] = 1;
                    QProcess *process = new QProcess();
                    QString file = "//home//pavo//BitmapCreator_rev03041901";
                    QStringList argList;
                    argList << topic << QString("%1").arg(reklamID) << QString("%1").arg(pageID) << QString("%1").arg((quint8) data.at(5))<< QString("%1").arg((quint8) data.at(6));
                    process->start(file,argList);
                }else{
                    writeDebug(tr("KanalMesgul (%1)").arg(pageID));
                    postData(data,tr("test.php?t=reset&a=%1&p=%2").arg(reklamID).arg(pageID));

                }
            }
                break;

            default                                 : writeDebug(tr("(%1-%2) UnknownMsgType"              ).arg(reklamID).arg(pageID).arg(msgType) );                                                                       break;
            }
        }
    }else{
        writeDebug(tr("onRecv %1 %2").arg(topic).arg(tempWrite));

        writeDebug("SOH not found !");
            }


}

void KonsantratorListener::postData(QByteArray sendData, QString Path){
    QSslSocket::supportsSsl();
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    QNetworkConfigurationManager manager;
    QNetworkRequest req (QUrl("http://localhost:62000/" + Path));

    //    qDebug()<<req.url().toString();

    req.setHeader(QNetworkRequest::UserAgentHeader,QVariant("akilliEtiketAgent"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/pavoApp");
    req.setHeader(QNetworkRequest::ContentLengthHeader,QVariant(sendData.size()));

    nam->clearAccessCache();
    nam->setConfiguration(manager.defaultConfiguration());
    nam->post(req,sendData);

    connect(nam,SIGNAL(finished(QNetworkReply*)),nam,SLOT(deleteLater()));
    connect(nam,SIGNAL(finished(QNetworkReply*)),this,SLOT(getResponse(QNetworkReply*)));

}
void KonsantratorListener::writeDebug(QString message){
    QFile file ("konsantrator_logs//"+KonsantratorID+".txt");

    if(file.open(QIODevice::Append)){
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString("hh:mm:ss.zzz dd/MM/yyyy")<<"\t" << message << endl;
        file.close();
    }else{
        qDebug()<<"LogError:" << file.errorString();
    }
    qDebug()<<QDateTime::currentDateTime().toString("hh:mm:ss.zzz dd/MM/yyyy")<<"\t" << message;
}
void KonsantratorListener::getResponse(QNetworkReply* reply){
    //    qDebug()<<reply->readAll();


    writeDebug(tr("WebResponse: %1").arg(QString(reply->readAll())));

    reply->close();


}
