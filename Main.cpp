// ConsoleApplication179.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <vector>
#include <deque>


namespace Commands
{
    class Command
    {
    public:
        virtual void run() {};

        Command() {};
    };

    class Nop : Command
    {
    public:
        void run() override {};
        Nop() {};
    };

    class Log : Command
    {
    public:
        void run() override { std::cout << "I'm logger."; };
        Log() {};
    };

    class Summuter : Command
    {
    public:
        void run() override { std::cout << "Sum 2 + 2 = 4."; };
        Summuter() {};
    };
}

namespace DynamicBlock
{
    class MainDynamicBlock
    {
    public:
        virtual void append(MainDynamicBlock*) {};
        virtual void append(Commands::Command*) {};
        virtual void run() {};
    };

    class DynamicBlockCommand : MainDynamicBlock
    {
        Commands::Command* command;
    public:
        DynamicBlockCommand()
        {
            command = (Commands::Command*)(new Commands::Nop());
        };

        DynamicBlockCommand(Commands::Command* cm)
        {
            command = cm;
        };

        void run()
        {
            command->run();
        }
    };

    class DynamicBlockEditor : MainDynamicBlock
    {
        std::deque<MainDynamicBlock*> commands;
        DynamicBlockEditor() {};

        void append(MainDynamicBlock* cm)
        {
            commands.push_back(cm);
        }

        void append(Commands::Command* cm)
        {
            commands.push_back((MainDynamicBlock*)(new DynamicBlockCommand(cm)));
        }

        void run()
        {
            for (auto i : commands)
            {
                i->run();

            }
        }
    };


}

namespace StaticBlock
{
    class StaticBlock
    {
        int N;
        std::vector<Commands::Command*> commands;
    public:
        StaticBlock(int N) : N(N) { commands.reserve(N); };

        void append(Commands::Command* cm)
        {
            commands.push_back(cm);
        }

        void run()
        {
            for (auto i : commands)
            {
                i->run();
            }
        }
    };
}

namespace Adapters
{

    class BlockAdapter
    {
    public:
        int N;
        bool is_it_command = false;
        virtual void append(BlockAdapter* ba) {};
        virtual void run() {};
    };

    class CommandBlockAdapter : public BlockAdapter
    {
        Commands::Command* cm;
    public:
        CommandBlockAdapter()
        {
            is_it_command = true;
            cm = new Commands::Command();
        };

        CommandBlockAdapter(Commands::Command* _cm)
        {
            is_it_command = true;
            cm = _cm;
        };

        Commands::Command* get()
        {
            return cm;
        }

        void run()
        {
            cm->run();
        }
    };

    class DynamicBlockAdapter : public BlockAdapter
    {
        DynamicBlock::MainDynamicBlock* db;
    public:

        DynamicBlockAdapter(int N)
        {

        }

        DynamicBlock::MainDynamicBlock* get()
        {
            return db;
        }

        void append(BlockAdapter* cm) override
        {
            if (cm->is_it_command == true)
            {
                db->append(((CommandBlockAdapter*)(cm))->get());
            }
            db->append(((DynamicBlockAdapter*)(cm))->get());
        }

        void run()
        {
            db->run();
        }
    };

    class StaticBlockAdapter : public BlockAdapter
    {
        StaticBlock::StaticBlock sb;
    public:
        StaticBlockAdapter(int N) : sb(N)
        {

        }


        StaticBlock::StaticBlock get()
        {
            return sb;
        }

        void append(BlockAdapter* cm) override
        {
            sb.append(((CommandBlockAdapter*)(cm))->get());
        }

        void run()
        {
            sb.run();
        }
    };

}

class ParserBlock
{
    ParserBlock* ParserBlock_high_up;
    Adapters::BlockAdapter* command;
    int N;
    int zanyato;

public:
    ParserBlock(int N) : N(N)
    {

    }

    void parse_string_to_command_and_save_it(std::string& s)
    {
        if (s[0] == '[')
        {
            ParserBlock pb{ N };
            pb.ParserBlock_high_up = this;
            command = pb.command;
            ParserBlock_high_up = pb.ParserBlock_high_up;
            return;
        }
        else if (s[0] == ']')
        {
            command->run();
            return;
        }
        else
        {
            if (zanyato == N)
            {
                command->run();
                zanyato = 0;
            }
        }

        if (s == "Log")
        {
            Commands::Log lg{};
            Commands::Command* cm = (Commands::Command*)(&lg);
            Adapters::CommandBlockAdapter ca{cm};
        }

        if (s == "Summer")
        {
            Commands::Summuter sm;
            Commands::Command* cm = (Commands::Command*)(&sm);
            Adapters::CommandBlockAdapter ca{ cm };
        }
        //Parse command
    }

    Adapters::BlockAdapter* get()
    {
        return command;
    }

    ParserBlock* get_parent()
    {
        return ParserBlock_high_up;
    }
};

int main(int argc, char** argv)
{
    int N = std::atoi((const char*)argv[1]);

    ParserBlock pr(N);

    std::string s;


    while (!std::cin.bad())
    {
        std::cin >> s;
        pr.parse_string_to_command_and_save_it(s);
        auto block = pr.get();
        auto parent_block = pr.get_parent()->get();
        parent_block->append(block);
    }

    std::cout << "Hello W1orld!\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
