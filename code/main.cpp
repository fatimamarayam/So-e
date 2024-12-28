#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <algorithm>
#include <sstream>

// Builder: Room
class room {
public:
    std::string room_name;
    int room_capacity;

    room(const std::string& name, int capacity) : room_name(name), room_capacity(capacity) {}
};

// Forward Declaration of bug Class
class bug {
public:
    virtual void show_traits() = 0;
    virtual ~bug() {}
};

// Builder: Hive
class hive {
    std::string hive_type;
    int coord_x, coord_y;
    int food_stock;
    bool leader_alive;
    std::vector<std::shared_ptr<room>> room_list;
    std::vector<std::shared_ptr<bug>> bug_list;

public:
    hive(const std::string& type, int x, int y) : hive_type(type), coord_x(x), coord_y(y), food_stock(100), leader_alive(true) {}

    void add_room(const std::string& name, int capacity) {
        room_list.push_back(std::make_shared<room>(name, capacity));
    }

    void add_bug(std::shared_ptr<bug> new_bug) {
        bug_list.push_back(new_bug);
    }

    void gather_resources(const std::string& resource, int quantity) {
        if (resource == "food") {
            food_stock += quantity;
            std::cout << "Food stock increased by " << quantity << ". Total: " << food_stock << std::endl;
        } else {
            std::cout << "Unknown resource: " << resource << std::endl;
        }
    }

    void process_tick() {
        if (leader_alive) {
            food_stock -= 5; // Basic food consumption
            if (food_stock <= 0) {
                leader_alive = false;
                std::cout << "The leader of this hive has died of starvation." << std::endl;
            }
        }
    }

    void display_info() {
        std::cout << "Type: " << hive_type << "\n";
        std::cout << "Coordinates: (" << coord_x << ", " << coord_y << ")\n";
        std::cout << "Food Stock: " << food_stock << "\n";
        std::cout << "Leader Alive: " << (leader_alive ? "Yes" : "No") << "\n";
        std::cout << "Rooms: " << room_list.size() << "\n";
        std::cout << "Bugs: " << bug_list.size() << "\n";
    }
};

// Singleton: Field Class
class field {
private:
    static field* field_instance;
    std::vector<std::shared_ptr<hive>> hive_list;
    field() {}

public:
    static field* get_instance() {
        if (!field_instance) {
            field_instance = new field();
        }
        return field_instance;
    }

    void create_hive(const std::string& type, int x, int y) {
        auto new_hive = std::make_shared<hive>(type, x, y);
        hive_list.push_back(new_hive);
        std::cout << "Hive of type " << type << " created at (" << x << ", " << y << ").\n";
    }

    void allocate_resources(int hive_id, const std::string& resource, int quantity) {
        if (hive_id >= 0 && hive_id < hive_list.size()) {
            hive_list[hive_id]->gather_resources(resource, quantity);
        } else {
            std::cout << "Invalid hive ID." << std::endl;
        }
    }

    void run_ticks(int tick_count) {
        for (int i = 0; i < tick_count; ++i) {
            std::cout << "Running Tick: " << i + 1 << std::endl;
            for (auto& hive : hive_list) {
                hive->process_tick();
            }
        }
    }

    void show_summary(int hive_id) {
        if (hive_id >= 0 && hive_id < hive_list.size()) {
            hive_list[hive_id]->display_info();
        } else {
            std::cout << "Invalid hive ID." << std::endl;
        }
    }
};

field* field::field_instance = nullptr;

// Decorator: bug Traits
class basic_bug : public bug {
public:
    void show_traits() override {
        std::cout << "Basic Bug Traits" << std::endl;
    }
};

class strong_bug : public bug {
    std::shared_ptr<bug> base_bug;

public:
    strong_bug(std::shared_ptr<bug> bug) : base_bug(bug) {}

    void show_traits() override {
        base_bug->show_traits();
        std::cout << " + Strong" << std::endl;
    }
};

class smart_bug : public bug {
    std::shared_ptr<bug> base_bug;

public:
    smart_bug(std::shared_ptr<bug> bug) : base_bug(bug) {}

    void show_traits() override {
        base_bug->show_traits();
        std::cout << " + Smart" << std::endl;
    }
};

// Factory: Create Bugs
class bug_factory {
public:
    enum bug_type { worker_bug, fighter_bug };

    static std::shared_ptr<bug> make_bug(bug_type type) {
        if (type == worker_bug) {
            return std::make_shared<basic_bug>();
        } else if (type == fighter_bug) {
            return std::make_shared<strong_bug>(std::make_shared<basic_bug>());
        }
        return nullptr;
    }
};

// Main Function
int main() {
    field* main_field = field::get_instance();

    std::cout << "Welcome to the Hive Management System!" << std::endl;
    std::cout << "Available Commands:" << std::endl;
    std::cout << "  spawn <x> <y> <type> - Create a new hive at coordinates (x, y) with the given type." << std::endl;
    std::cout << "  give <hive_id> <resource> <quantity> - Allocate resources to a hive." << std::endl;
    std::cout << "  tick <quantity> - Process a number of ticks (default: 1)." << std::endl;
    std::cout << "  summary <hive_id> - Show summary of a specific hive." << std::endl;
    std::cout << "  quit - Exit the program." << std::endl;

    std::string input_line;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input_line);

        std::istringstream input_stream(input_line);
        std::string command;
        input_stream >> command;

        if (command == "quit") {
            std::cout << "Exiting the Hive Management System. Goodbye!" << std::endl;
            break;
        }

        int x, y, id, quantity;
        std::string resource, type;

        if (command == "spawn") {
            input_stream >> x >> y >> type;
            if (!input_stream.fail()) {
                main_field->create_hive(type, x, y);
            } else {
                std::cout << "Invalid input. Usage: spawn <x> <y> <type>" << std::endl;
            }
        } else if (command == "give") {
            input_stream >> id >> resource >> quantity;
            if (!input_stream.fail() && quantity > 0) {
                main_field->allocate_resources(id, resource, quantity);
            } else {
                std::cout << "Invalid input. Usage: give <hive_id> <resource> <quantity>" << std::endl;
            }
        } else if (command == "tick") {
            input_stream >> quantity;
            if (input_stream.fail()) {
                quantity = 1; // Default to 1 tick if not specified
            }
            if (quantity > 0) {
                main_field->run_ticks(quantity);
            } else {
                std::cout << "Invalid input. Quantity must be greater than 0." << std::endl;
            }
        } else if (command == "summary") {
            input_stream >> id;
            if (!input_stream.fail()) {
                main_field->show_summary(id);
            } else {
                std::cout << "Invalid input. Usage: summary <hive_id>" << std::endl;
            }
        } else {
            std::cout << "Unknown command. Type one of the following: spawn, give, tick, summary, quit." << std::endl;
        }
    }

    return 0;
}
