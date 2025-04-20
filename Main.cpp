#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <memory>

namespace Commands {
    class Command {
    public:
        virtual void run() = 0;
        virtual ~Command() = default;
    };

    class Log : public Command {
    public:
        void run() override { std::cout << "I'm logger.\n"; }
    };

    class Summuter : public Command {
    public:
        void run() override { std::cout << "Sum 2 + 2 = 4.\n"; }
    };
}

class Block {
protected:
    std::vector<std::unique_ptr<Commands::Command>> commands;
public:
    virtual void add_command(std::unique_ptr<Commands::Command> cmd) {
        commands.push_back(std::move(cmd));
    }
    virtual void execute() {
        for (auto& cmd : commands) {
            if (cmd) cmd->run();
        }
        commands.clear();
    }
    virtual ~Block() = default;
};

class StaticBlock : public Block {
    int capacity;
    int count = 0;
public:
    StaticBlock(int N) : capacity(N) {}

    void add_command(std::unique_ptr<Commands::Command> cmd) override {
        Block::add_command(std::move(cmd));
        if (++count >= capacity) {
            execute();
            count = 0;
        }
    }
};

class DynamicBlock : public Block {
public:
    DynamicBlock* parent;
    void execute() override {
        Block::execute();
    }
};

class Parser {
    std::unique_ptr<StaticBlock> static_block;
    DynamicBlock* dynamic_blocks;
    int block_size;

public:
    Parser(int N) : block_size(N) {
        static_block = std::make_unique<StaticBlock>(N);
    }

    void parse(const std::string& input) {
        if (input == "[") {
            // Начало динамического блока
            DynamicBlock* db = new DynamicBlock();
            db->parent = dynamic_blocks;
            dynamic_blocks = db;
        }
        else if (input == "]") {
            // Конец динамического блока
            if (dynamic_blocks != nullptr)
            {
                DynamicBlock* parent = dynamic_blocks->parent;
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
            else if (input == "Summer") {
                cmd = std::make_unique<Commands::Summuter>();
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
};

int main(int argc, char** argv) {

    int N = 5;
    Parser parser(N);
    std::string input;

    while (std::cin >> input) {
        parser.parse(input);
    }

    return 0;
}
