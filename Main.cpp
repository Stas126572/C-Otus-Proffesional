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

    class Log : public Command {
    public:
        void run() override { ss << "Log"; }
    };

    class Sum : public Command {
    public:
        void run() override { ss << "Sum"; }
    };
}

class ConsoleExecute
{
    bool IsMeWrite = false;
public:
    void update() { IsMeWrite = false; };
    void operator() (std::stringstream& ss)
    {
        if (!IsMeWrite)
        {
            std::cout << "bulk:";
            IsMeWrite = true;
        }
        else
        {
            std::cout << ",";
        }
        std::cout << " " << ss.str();
    }
};

class FileExecute
{
    size_t time;
    std::ofstream ofs;
    std::string name;
    bool IsItWrite;
public:
    void update()
    {
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
    void operator() (std::stringstream& ss)
    {

        ofs.open(name, std::ios::app);
        if (!IsItWrite)
        {
            ofs << "bulk:";
        }
        else
        {
            ofs << ",";
        }
        
        ofs << " " << ss.str();
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
    }
};
template<typename T>
class Parser {
    std::unique_ptr<StaticBlock<T>> static_block;
    DynamicBlock<T>* dynamic_blocks;
    int block_size;

public:
    Parser(int N) : block_size(N) {
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
            if (input == "Log") {
                cmd = std::make_unique<Commands::Log>();
            }
            else if (input == "Sum") {
                cmd = std::make_unique<Commands::Sum>();
            }

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
