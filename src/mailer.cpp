//
// mailer.cpp
//

#include <string>
#include <iostream>

#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/StringPartSource.h"

#include "../include/mailer.h"

//-------------------------------------------------------------------------

Email::Email(std::string _mailhost,std::string _sender,std::string _recipient):
    mailhost {_mailhost},
    sender {_sender},
    recipient {_recipient}
{
};

//-------------------------------------------------------------------------

void Email::set_subject(std::string sub)
{
    message.setSubject(sub);
}

//-------------------------------------------------------------------------

void Email::set_content(std::string msg)
{
    message.addContent(new Poco::Net::StringPartSource(msg));
}

//-------------------------------------------------------------------------

void Email::send()
{
    message.setSender(sender);
    message.addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,recipient));
    Poco::Net::SMTPClientSession session(mailhost);
    session.login();
    session.sendMessage(message);
    session.close();
}
