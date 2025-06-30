#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <sstream>
#include <fstream>
#include <ctime>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <bitset>
#include <barrier>
void run();
void console_run();

size_t connect(size_t N);
void receive(const std::string& input, size_t id);
std::thread file1(run);
std::thread file2(run);
std::thread _log(console_run);
