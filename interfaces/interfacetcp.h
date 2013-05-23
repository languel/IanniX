#ifndef INTERFACETCP_H
#define INTERFACETCP_H

#include <QTcpServer>
#include <QDomDocument>
#include "misc/options.h"
#include "messages/messagemanager.h"

namespace Ui {
class InterfaceTcp;
}

class InterfaceTcpServer : public QTcpServer {
    Q_OBJECT

public:
    InterfaceTcpServer(QObject *parent);

public:
    QList<QTcpSocket*> sockets;
    bool send(const Message & );
public:
    bool portChanged(quint16 port);
protected:
    void incomingConnection(int socketDescriptor);
private slots:
    void readClient();
    void discardClient();
signals:
    void updateConnectedClients();
    void parseXml(const QDomDocument&, QTcpSocket*);
};

class InterfaceTcp : public NetworkInterface {
    Q_OBJECT
    
public:
    explicit InterfaceTcp(QWidget *parent = 0);
    ~InterfaceTcp();

private:
    InterfaceTcpServer *tcpServer;
    UiReal port;
    UiBool enable;
private slots:
    void portChanged();
    void updateConnectedClients();
    void parseXml(const QDomDocument&, QTcpSocket*);
    void openExamples() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo("Patches/Adobe Flash/").absoluteFilePath()));
    }

public:
    bool send(const Message & );

private:
    Ui::InterfaceTcp *ui;
};

#endif // INTERFACETCP_H
