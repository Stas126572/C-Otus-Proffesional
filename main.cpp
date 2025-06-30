#include "lib.h"


int main() {
    // Пример использования
    size_t conn_id = connect(3); // Блок из 3 команд

    receive("cmd1", conn_id);
    receive("cmd2", conn_id);
    receive("cmd3", conn_id); // Должен выполниться статический блок

    receive("{", conn_id);
    receive("cmd4", conn_id);
    receive("cmd5", conn_id);
    receive("}", conn_id); // Должен выполниться динамический блок

    // Даем потокам время завершить работу
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
