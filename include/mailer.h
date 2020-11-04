//
// mailer.h
//

#ifndef MAILER_H
#define MAILER_H

#include <string>
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"

class Email
{
public:
    Email (std::string _mailhost,std::string _sender,std::string _recipient);
    void set_subject(std::string sub);
    void set_content(std::string msg);
    void send();

private:
    std::string subject;
    std::string content;
    Poco::Net::MailMessage message;
    std::string mailhost;
    std::string sender;
    std::string recipient;
};

#endif //MAILER_H
