#include "FrmAddRoster.h"
#include "ui_FrmAddRoster.h"
#include "../../XmppClient.h"
#include "qxmpp/QXmppRosterManager.h"
#include "../../Global.h"
#include <string>
#include <QDebug>

CFrmAddRoster::CFrmAddRoster(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CFrmAddRoster)
{
    ui->setupUi(this);
    ui->txtGroup->setEditable(true);
    m_bRequest = false;
}

CFrmAddRoster::~CFrmAddRoster()
{
    delete ui;
}

int CFrmAddRoster::Init(CXmppClient *pClient, QSet<QString> groups, QString bareJid)
{
    ui->txtJID->clear();
    ui->txtNick->clear();
    ui->txtGroup->clear();
    if(!bareJid.isEmpty())
    {
        this->setWindowTitle(tr("Request add roster"));
        ui->lbPrompt->setText(tr("%1 request add roster").arg(bareJid));
        ui->txtJID->setEnabled(false);
        ui->txtJID->setText(bareJid);
        on_txtJID_editingFinished();
        m_bRequest = true;
    }
    else
    {
        this->setWindowTitle(tr("Add Roster"));
        ui->lbPrompt->setText(tr("Add Roster"));
        ui->txtJID->setEnabled(true);
        m_bRequest = false;
    }
    if(groups.isEmpty())
    {
        ui->txtGroup->setCurrentText(tr("My friends"));
    }
    else
    {
        foreach(QString g, groups)
        {
            ui->txtGroup->addItem(g);
        }
    }
    m_pClient = pClient;
    return 0;
}

void CFrmAddRoster::on_pbOk_clicked()
{
    QString szJid = ui->txtJID->text();
    if(-1 == szJid.indexOf("@"))
        szJid += "@rabbitim.com";
    QString szNick = ui->txtNick->text();
    QSet<QString> groups;
    groups << ui->txtGroup->currentText();
    if(m_bRequest)
    {
        m_pClient->rosterManager().acceptSubscription(szJid);
    }
    m_pClient->rosterManager().subscribe(szJid);
    m_pClient->rosterManager().addItem(szJid, szNick, groups);
    this->close();
}

void CFrmAddRoster::on_pushButton_clicked()
{
    if(m_bRequest)
        m_pClient->rosterManager().refuseSubscription(ui->txtJID->text());
    this->close();
}

void CFrmAddRoster::on_txtJID_editingFinished()
{
    qDebug() << "CFrmAddRoster::on_txtJID_editingFinished";
    if(ui->txtNick->text().isEmpty())
    {
        QString szJid = ui->txtJID->text();
        int nPos = szJid.indexOf("@");
        if(nPos > 0)
            ui->txtNick->setText(szJid.left(nPos));
        else
            ui->txtNick->setText(szJid);
    }
}