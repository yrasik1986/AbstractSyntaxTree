#include <iostream>
#include <string>

using namespace std;

void SendSms(const string& number, const string& message) {
    cout << "Send '" << message << "' to number " << number << endl;
}

void SendEmail(const string& email, const string& message) {
    cout << "Send '" << message << "' to e-mail " << email << endl;
}

class INotifier {
public:
    virtual void Notify(const string& message) const = 0;
};

class SmsNotifier : public INotifier {
public:
	SmsNotifier (const string& tel) : _tel(tel) {}

    void Notify(const string& message) const override {
        SendSms(_tel, message);
    }
	
private:
     const string _tel;
};

class EmailNotifier : public INotifier {
public:
    EmailNotifier(const string& email) : _email(email) {}

    void Notify(const string& message) const override {
        SendEmail(_email, message);
    }
	
private:
    const string _email;
};


/*
 ���������� ����� ������ INotifier, SmsNotifier, EmailNotifier
 */

void Notify(INotifier& notifier, const string& message) {
    notifier.Notify(message);
}


int main() {
    SmsNotifier sms{ "+7-495-777-77-77" };
    EmailNotifier email{ "na-derevnyu@dedushke.ru" };

    Notify(sms, "I have White belt in C++");
    Notify(email, "And want a Yellow one");
    return 0;
}