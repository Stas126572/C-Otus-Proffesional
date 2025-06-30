#include "async.h"

thread_local std::ofstream ofs;
bool IsItFinallyVersion = true;
thread_local bool is_it_first = true;

void really_execute_file(std::string ss, size_t time_) {
    std::string name = "bulk";
    std::string id;
    std::stringstream ss_;
    ss_ << std::this_thread::get_id();
    ss_ >> id;

    name += id;
    name += std::to_string(time_);
    name += ".log";

    ofs.open(name, std::ios::app);
    if (IsItFinallyVersion) {
        ofs << "bulk: ";
    }
    ofs << ss << ", ";
    ofs.close();
}

void really_execute_console(std::string ss) {
    if (IsItFinallyVersion) {
        std::cout << "bulk: ";
    }
    std::cout << ss << ", ";
}

namespace Blocks {
struct StaticBlock {
    int N;
    std::vector<std::string> commands;
    StaticBlock() = default;
    StaticBlock(int N) : N(N) {};

    void append_command(std::string cmd) {
        commands.push_back(cmd);
    }

    bool NeedToRun() const {
        return commands.size() >= static_cast<size_t>(N);
    }
};

struct DynamicBlock {
    int level_in = 0;
    std::vector<std::string> commands;
    DynamicBlock() = default;
    DynamicBlock(int level_in) : level_in(level_in) {};

    void append_command(std::string cmd) {
        commands.push_back(cmd);
    }

    void minus_one_level() {
        if (level_in > 0) {
            level_in--;
        }
    }

    bool NeedToRun() const {
        return (level_in == 0) && !commands.empty();
    }
};
}

std::map<size_t, std::pair<std::unique_ptr<Blocks::StaticBlock>, std::unique_ptr<Blocks::DynamicBlock>>> command;
std::map<size_t, std::vector<std::bitset<8>>> task_pull;
std::mutex _mt;
std::thread::id first_thread;
unsigned char use_bits = 0;
std::condition_variable cv;
std::barrier my_barrier{ 3 };

size_t time_id = 0;
bool IsItReally = false;

void set_needly_bit() {
    if (std::this_thread::get_id() == first_thread) {
        std::lock_guard<std::mutex> lock(_mt);
        if (use_bits < 8) {
            task_pull[time_id].back().set(use_bits);
            use_bits++;
        }
        else {
            task_pull[time_id].push_back(0);
            use_bits = 0;
        }
    }
}

void wait_while_all_stopping() {
    my_barrier.arrive_and_wait();
}

void run() {
    first_thread = std::this_thread::get_id();
    while (true) {
        std::unique_lock<std::mutex> lock(_mt);
        cv.wait(lock, [] { return IsItReally; });

        if (command[time_id].second->level_in > 0) {
            // Dynamic block
            if (command[time_id].second->NeedToRun()) {
                lock.unlock();

                for (size_t i = 0; i < task_pull[time_id].size(); i++) {
                    for (int j = 0; j < 8; j++) {
                        if (i == task_pull[time_id].size() - 1 && j >= use_bits) {
                            break;
                        }
                        if (task_pull[time_id][i].test(j) ^ (std::this_thread::get_id() != first_thread)) {
                            really_execute_file(command[time_id].second->commands[i * 8 + j], time_id);
                        }
                    }
                }
                wait_while_all_stopping();
            }
            else {
                set_needly_bit();
            }
        }
        else {
            // Static block
            if (command[time_id].first->NeedToRun()) {
                lock.unlock();

                for (size_t i = 0; i < task_pull[time_id].size(); i++) {
                    for (int j = 0; j < 8; j++) {
                        if (i == task_pull[time_id].size() - 1 && j >= use_bits) {
                            break;
                        }
                        if (task_pull[time_id][i].test(j) ^ (std::this_thread::get_id() != first_thread)) {
                            really_execute_file(command[time_id].first->commands[i * 8 + j], time_id);
                        }
                    }
                }
                wait_while_all_stopping();
            }
            else {
                set_needly_bit();
            }
        }
    }
}

void console_run() {
    while (true) {
        std::unique_lock<std::mutex> lock(_mt);
        cv.wait(lock, [] { return IsItReally; });

        if (command[time_id].second->level_in > 0) {
            if (command[time_id].second->NeedToRun()) {
                lock.unlock();
                for (const auto& cmd : command[time_id].second->commands) {
                    really_execute_console(cmd);
                }
                wait_while_all_stopping();
                command[time_id].second->commands.clear();
            }
        }
        else {
            if (command[time_id].first->NeedToRun()) {
                lock.unlock();
                for (const auto& cmd : command[time_id].first->commands) {
                    really_execute_console(cmd);
                }
                wait_while_all_stopping();
                command[time_id].first->commands.clear();
            }
        }
    }
}

size_t connect(size_t N) {
    size_t time_ = std::time(nullptr);
    std::lock_guard<std::mutex> lock(_mt);

    command[time_] = std::make_pair(
        std::make_unique<Blocks::StaticBlock>(N),
        std::make_unique<Blocks::DynamicBlock>()
        );
    task_pull[time_].push_back(0);
    use_bits = 0;

    return time_;
}

void receive(const std::string& input, size_t id) {
    std::lock_guard<std::mutex> lock(_mt);

    if (input == "{") {
        command[id].second->level_in++;
    }
    else if (input == "}") {
        command[id].second->minus_one_level();
        if (command[id].second->NeedToRun()) {
            time_id = id;
            IsItReally = true;
            cv.notify_all();
        }
    }
    else {
        if (command[id].second->level_in > 0) {
            command[id].second->append_command(input);
        }
        else {
            command[id].first->append_command(input);
            if (command[id].first->NeedToRun()) {
                time_id = id;
                IsItReally = true;
                cv.notify_all();
            }
        }
    }
}
