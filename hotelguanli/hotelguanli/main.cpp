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
    // 显示房间状态
    Hotel(int numRooms):rooms(numRooms,false){}

    void displayRooms()const
    {
        cout<<"房间状态"<<endl;
        for(int i=0;i<rooms.size();i++)
        {
            cout<<"房间"<<i+1<<":"<<(rooms[i]?"已预定":"空闲")<<endl;
        }
    }
    //预订房间 
    bool bookRoom(int roomNumber)
    {
        if (roomNumber < 1 || roomNumber > rooms.size()) {
            std::cout << "无效的房间号。\n";
            return false;
        }
        if (rooms[roomNumber - 1]) {
            std::cout << "房间 " << roomNumber << " 已被预订。\n";
            return false;
        }
        rooms[roomNumber - 1] = true;
        std::cout << "房间 " << roomNumber << " 预订成功。\n";
        return true;
    }
    //退房
    bool checkoutRoom(int roomNumber)
    {
        if (roomNumber < 1 || roomNumber > rooms.size()) {
            std::cout << "无效的房间号。\n";
            return false;
        }
        if (!rooms[roomNumber - 1]) {
            std::cout << "房间 " << roomNumber << " 已经是空闲状态。\n";
            return false;
        }
        rooms[roomNumber - 1] = false;
        std::cout << "房间 " << roomNumber << " 退房成功。\n";
        return true;
    }

    //保存房间状态到文件
    void saveState(const string& filename) const
    {
        ofstream file(filename);
        for(bool room:rooms)
        {
            file<<room<<endl;
        }
        file.close();
    }


    //从文件加载房间状态
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

//用户管理，注册，登陆
class UserManager
{
private:
    map<string,User> users;
    string currentUser;

public:
// 注册新用户
    bool regisUser(const string& username,const string& password)
    {
        if(users.find(username)!=users.end())
        {
            cout<<"用户已存在"<<endl;
            return false;
        }
        users[username]=User(username,password);
        cout<<"注册成功"<<endl;
        return true;
    }

// 用户登陆
    bool login(const string& username,const string& password)
    {
        if(users.find(username)==users.end()||users[username].password!=password)
        {
                cout<<"用户名或密码错误"<<endl;
                return false;
        }
        currentUser=username;
        cout<<"登陆成功"<<username<<endl;
        return true;
    }
   
    //用户是否登陆
    bool isLogin() const
    {
        return !currentUser.empty();
    }
    //保存用户数据
    void saveUsers(const string& filename)const
    {
        ofstream file(filename);
        for(auto& user:users)
        {
            file << user.second.username<<" "<<user.second.password<<endl;
        }
        file.close();
    }

    //从文件中加载用户数据
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

    //加载用户和房间信息
    userManager.loadUsers(userFile);
    hotel.loadState(roomFile);
    printLogo();
    while(1)
    {
        if(!userManager.isLogin())
        {
            cout << "1. 登录\n";
            cout << "2. 注册\n";
            cout << "3. 退出\n";
            cout << "请选择操作: ";
            int choice;
            cin>>choice;
            if(choice==1)
            {
                string username,password;
                cout<<"请输入用户名"<<endl;
                cin>>username;
                cout<<"请输入密码"<<endl;
                cin>>password;
                userManager.login(username,password);
            }
            else if(choice==2)
            {
                string username,password;
                cout<<"请输入用户名"<<endl;
                cin>>username;
                cout<<"请输入密码"<<endl;
                cin>>password;
                userManager.regisUser(username,password);
            }
            else if(choice==3)
            {
                break;
            }
            else{
                cout<<"请重新选择"<<endl;
            }
        }
        else{
            cout<<"酒店入住管理系统"<<endl;
            cout << "1. 查看所有房间状态\n";
            cout << "2. 预订房间\n";
            cout << "3. 退房\n";
            cout << "4. 退出\n";
            cout << "请选择操作: ";

            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1:
                    hotel.displayRooms();
                    break;
                case 2: {
                    int roomNumber;
                    cout << "请输入要预订的房间号: ";
                    cin >> roomNumber;
                    hotel.bookRoom(roomNumber);
                    break;
                }
                case 3: {
                    int roomNumber;
                    std::cout << "请输入要退房的房间号: ";
                    std::cin >> roomNumber;
                    hotel.checkoutRoom(roomNumber);
                    break;
                }
                case 4:
                    // 保存数据并退出
                    userManager.saveUsers(userFile);
                    hotel.saveState(roomFile);
                    std::cout << "退出系统。\n";
                    return 0;
                    default:
                    std::cout << "无效的选择，请重新输入。\n";
            }
        }
    }
    return 0;
}