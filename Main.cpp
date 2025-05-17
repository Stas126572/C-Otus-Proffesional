#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string>

namespace Commands {
    class Command {
    public:
        std::stringstream ss;
        virtual void run() = 0;
        virtual ~Command() = default;
    };

    class PrintCommand : public Command
    {
        std::string s;
    public:
        PrintCommand(std::string s) : s(s)
        {

        }
        void run() override {
            ss << s;
        }
    };
}

class BasiceExecute
{
public:
    std::stringstream ss;
    virtual void really_execute() {};
};

class ConsoleExecute : public BasiceExecute
{
    bool IsMeWrite = false;
public:
    void update() 
    {
        ss.clear();//clear any bits set
        ss.str(std::string());
        IsMeWrite = false;
    };
    void operator() (std::stringstream& ss_)
    {
        if (!IsMeWrite)
        {
            ss << "bulk:";
            IsMeWrite = true;
        }
        else
        {
            ss << ",";
        }
        ss << " " << ss_.str();
    }

    void really_execute() override
    {
        std::cout << ss.str();
    }

    std::string get_my_buf()
    {
        return ss.str();
    }
};

class FileExecute : public BasiceExecute
{
    size_t time;
    std::ofstream ofs;
    std::string name;
    bool IsItWrite;
public:
    void update()
    {
        ss.clear();//clear any bits set
        ss.str(std::string());
        if (ofs.is_open())
        {
            ofs.close();
        }

        time = std::time(NULL);
        name = "bulk";
        name += std::to_string(time);
        name += ".log";
        IsItWrite = false;
    }
    void operator() (std::stringstream& ss_)
    {

        if (!IsItWrite)
        {
            ss << "bulk:";
            IsItWrite = true;
        }
        else
        {
            ss << ",";
        }

        ss << " " << ss_.str();
        ofs.close();
    }

    void really_execute() override
    {
        ofs.open(name, std::ios::app);
        ofs << ss.str();
        ofs.close();
    }

    

    ~FileExecute()
    {
        if (ofs.is_open())
        {
            ofs.close();
        }
    }
};

class Execute
{
    ConsoleExecute ce;
    FileExecute fe;
public:
    Execute()
    {
        update();
    }
    void update()
    {
        ce.update();
        fe.update();
    }

    void operator() (std::stringstream& ss)
    {
        ce(ss);
        fe(ss);
    }

    std::string get_my_buf()
    {
        return ce.get_my_buf();
    }

    void really_execute()
    {
        ce.really_execute();
        fe.really_execute();
    }
};

template<typename T>
class Block {
protected:

    std::vector<std::unique_ptr<Commands::Command>> commands;
    bool IsItExecute;
    T executable;
public:
    Block() : IsItExecute(false)
    {

    }

    virtual void add_command(std::unique_ptr<Commands::Command> cmd) {
        commands.push_back(std::move(cmd));
        if (!IsItExecute)
        {
            IsItExecute = true;
            executable.update();
        }
    }
    virtual void execute() {
        for (auto& cmd : commands) {
            if (cmd)
            {
                cmd->run();
                std::stringstream& ss_ = cmd->ss;
                executable(ss_);
                IsItExecute = false;
            }
        }

        commands.clear();
    }
    std::string get_buf()
    {
        return executable.get_my_buf();
    }
    virtual ~Block() = default;
};
template<typename T>
class StaticBlock : public Block<T> {
    int capacity;
    int count = 0;
public:
    StaticBlock(int N) : capacity(N) {}

    void add_command(std::unique_ptr<Commands::Command> cmd) override {
        Block<T>::add_command(std::move(cmd));
        if (++count >= capacity) {
            Block<T>::execute();
            Block<T>::executable.really_execute();
            count = 0;
        }
    }
};
template<typename T>
class DynamicBlock : public Block<T> {
public:
    DynamicBlock* parent;
    void execute() override {
        Block<T>::execute();
        if (parent == nullptr)
        {
            Block<T>::executable.really_execute();
        }
        else
        {
            auto l = Block<T>::get_buf();
            std::unique_ptr<Commands::Command> cmd;
            cmd = std::make_unique<Commands::PrintCommand>(l);
            parent->add_command(std::move(cmd));
        }
    }
};
template<typename T>
class Parser {
    std::unique_ptr<StaticBlock<T>> static_block;
    DynamicBlock<T>* dynamic_blocks;
    int block_size;

public:
    Parser(int N) : block_size(N) {
        dynamic_blocks = nullptr;
        static_block = std::make_unique<StaticBlock<T>>(N);
    }

    void parse(const std::string& input) {
        if (input == "{") {
            // Начало динамического блока
            DynamicBlock<T>* db = new DynamicBlock<T>();
            db->parent = dynamic_blocks;
            dynamic_blocks = db;
        }
        else if (input == "}") {
            // Конец динамического блока
            if (dynamic_blocks != nullptr)
            {
                DynamicBlock<T>* parent = dynamic_blocks->parent;
                dynamic_blocks->execute();
                delete dynamic_blocks;
                dynamic_blocks = parent;
            }
        }
        else {
            std::unique_ptr<Commands::Command> cmd;
            cmd = std::make_unique<Commands::PrintCommand>(input);

            if (cmd) {
                if (dynamic_blocks != nullptr) {
                    // Добавляем в текущий динамический блок
                    dynamic_blocks->add_command(std::move(cmd));
                }
                else {
                    // Добавляем в статический блок
                    static_block->add_command(std::move(cmd));
                }
            }
        }
    }

    ~Parser()
    {
        if (dynamic_blocks == nullptr)
        {
            static_block->execute();
        }
        else
        {
            delete dynamic_blocks;
        }
    }
};

int main(int argc, char** argv) {

     
    if (argc < 2)
    {
        std::cout << "Use: " << argv[0] << "with parameter N";
        return 1;
    }

    int N = atoi(argv[1]);
    Parser<Execute> parser{ N };
    std::string input;

    while (std::cin >> input) {
        parser.parse(input);
    }

    return 0;
}
