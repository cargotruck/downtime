DOWNTIME

downtime written by nicholas.flesch@outlook.com
last modified: 02 November 2020

downtime is a utility designed to help IT administrators
be alerted when important networked devices lose contact
with the network. Users can configure it to survey a list
of nodes on their network as often as they like. If a node
does not respond to a ping for a given number of intervals
(also user configured) an alert will be generated and emailed
to the designated email address.

The list of nodes and the configuration perameters can be
configured in the gui interface or by editing the address book
and config files in a text editor.

The address book syntax is: FQDN : IP address
downtime only requires one of these, the FQDN or the IP address.
However, the colon is required. Acceptable entries are:
computer.domain.com : 127.0.0.1
computer.domain.com :
 : 127.0.0.1

The required fields in the config file are:
email_host --> is the server alerts will be sent to. The value can be in FQDN or IP address format.
Example: FQDN: mail.example.com | IP: 192.168.1.230

sender_email --> is the email address you want displayed in the "From" field of an email.

recipient_email --> is the email address you want the alert delivered to.

Optional fields in the config file are:
wait_for --> is the period in seconds Downtime will wait between sending pings. DEFAULT: 60.

max_failes --> is the number of consecutive failed responses counted against a node
before an alert is generated. DEFAULT: 5.

log_dir --> is the directory where Downtime log files will be kept. DEFAULT: /opt/downtime/logs/.

Comments can be left in the config file by beginning the line with a hash symbol, '#'

REQUIREMENTS

Downtime is built for Linux.

You'll need your own email server for the 
email alerts.

downtime uses the wxWidgets library and the POCO
library. If compiling from source code these libraries
need to be installed on the computer. The libraries can
be found at:
http://wxwidgets.org/
https://pocoproject.org/
