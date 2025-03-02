#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <limits>
#include <vector>
using namespace std;

class User
{
    public:
    string username;
    string password;

    User()=default;
    User(const string& uname,const string& pwd)
    : username(uname), password(pwd){}
};

class Hotel
{
    private:
    vector<bool> rooms;

    public:
    // ��ʾ����״̬
    Hotel(int numRooms):rooms(numRooms,false){}

    void displayRooms()const
    {
        cout<<"����״̬"<<endl;
        for(int i=0;i<rooms.size();i++)
        {
            cout<<"����"<<i+1<<":"<<(rooms[i]?"��Ԥ��":"����")<<endl;
        }
    }
    //Ԥ������ 
    bool bookRoom(int roomNumber)
    {
        if (roomNumber < 1 || roomNumber > rooms.size()) {
            std::cout << "��Ч�ķ���š�\n";
            return false;
        }
        if (rooms[roomNumber - 1]) {
            std::cout << "���� " << roomNumber << " �ѱ�Ԥ����\n";
            return false;
        }
        rooms[roomNumber - 1] = true;
        std::cout << "���� " << roomNumber << " Ԥ���ɹ���\n";
        return true;
    }
    //�˷�
    bool checkoutRoom(int roomNumber)
    {
        if (roomNumber < 1 || roomNumber > rooms.size()) {
            std::cout << "��Ч�ķ���š�\n";
            return false;
        }
        if (!rooms[roomNumber - 1]) {
            std::cout << "���� " << roomNumber << " �Ѿ��ǿ���״̬��\n";
            return false;
        }
        rooms[roomNumber - 1] = false;
        std::cout << "���� " << roomNumber << " �˷��ɹ���\n";
        return true;
    }

    //���淿��״̬���ļ�
    void saveState(const string& filename) const
    {
        ofstream file(filename);
        for(bool room:rooms)
        {
            file<<room<<endl;
        }
        file.close();
    }


    //���ļ����ط���״̬
    void loadState(const string& filename)
    {
        ifstream file(filename);
        if(file.is_open())
        {
            int temp;
            for(int i=0;i<rooms.size();i++)
            {
                file >> temp;
                rooms[i]=temp;
            }
            file.close();
        }
    }
};

//�û�����ע�ᣬ��½
class UserManager
{
private:
    map<string,User> users;
    string currentUser;

public:
// ע�����û�
    bool regisUser(const string& username,const string& password)
    {
        if(users.find(username)!=users.end())
        {
            cout<<"�û��Ѵ���"<<endl;
            return false;
        }
        users[username]=User(username,password);
        cout<<"ע��ɹ�"<<endl;
        return true;
    }

// �û���½
    bool login(const string& username,const string& password)
    {
        if(users.find(username)==users.end()||users[username].password!=password)
        {
                cout<<"�û������������"<<endl;
                return false;
        }
        currentUser=username;
        cout<<"��½�ɹ�"<<username<<endl;
        return true;
    }
   
    //�û��Ƿ��½
    bool isLogin() const
    {
        return !currentUser.empty();
    }
    //�����û�����
    void saveUsers(const string& filename)const
    {
        ofstream file(filename);
        for(auto& user:users)
        {
            file << user.second.username<<" "<<user.second.password<<endl;
        }
        file.close();
    }

    //���ļ��м����û�����
    void loadUsers(const string& filename)
    {
        std::ifstream file(filename);
        if (file.is_open()) 
        {
            string username, password;
            while (file >> username >> password) {
                users[username] = User(username, password);
            }
            file.close();
        }
    }
};

void printLogo()
{
    cout<<"+--------------------------+"<<endl;
    cout<<"|          HOTEL           |"<<endl;
    cout<<"+--------------------------+"<<endl;
}

int main()
{
    const string userFile="users.txt";
    const string roomFile="rooms.txt";
    int roomNum=10;

    Hotel hotel(roomNum);
    UserManager userManager;

    //�����û��ͷ�����Ϣ
    userManager.loadUsers(userFile);
    hotel.loadState(roomFile);
    printLogo();
    while(1)
    {
        if(!userManager.isLogin())
        {
            cout << "1. ��¼\n";
            cout << "2. ע��\n";
            cout << "3. �˳�\n";
            cout << "��ѡ�����: ";
            int choice;
            cin>>choice;
            if(choice==1)
            {
                string username,password;
                cout<<"�������û���"<<endl;
                cin>>username;
                cout<<"����������"<<endl;
                cin>>password;
                userManager.login(username,password);
            }
            else if(choice==2)
            {
                string username,password;
                cout<<"�������û���"<<endl;
                cin>>username;
                cout<<"����������"<<endl;
                cin>>password;
                userManager.regisUser(username,password);
            }
            else if(choice==3)
            {
                break;
            }
            else{
                cout<<"������ѡ��"<<endl;
            }
        }
        else{
            cout<<"�Ƶ���ס����ϵͳ"<<endl;
            cout << "1. �鿴���з���״̬\n";
            cout << "2. Ԥ������\n";
            cout << "3. �˷�\n";
            cout << "4. �˳�\n";
            cout << "��ѡ�����: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    hotel.displayRooms();
                    break;
                case 2: {
                    int roomNumber;
                    cout << "������ҪԤ���ķ����: ";
                    cin >> roomNumber;
                    hotel.bookRoom(roomNumber);
                    break;
                }
                case 3: {
                    int roomNumber;
                    std::cout << "������Ҫ�˷��ķ����: ";
                    std::cin >> roomNumber;
                    hotel.checkoutRoom(roomNumber);
                    break;
                }
                case 4:
                    // �������ݲ��˳�
                    userManager.saveUsers(userFile);
                    hotel.saveState(roomFile);
                    std::cout << "�˳�ϵͳ��\n";
                    return 0;
                    default:
                    std::cout << "��Ч��ѡ�����������롣\n";
            }
        }
    }
    return 0;
}