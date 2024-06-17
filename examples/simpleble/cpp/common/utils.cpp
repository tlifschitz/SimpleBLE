#include "utils.hpp"

#include <iomanip>
#include <iostream>

void Utils::print_byte_array(const SimpleBLE::ByteArray& bytes) {
    for (auto b : bytes) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)((uint8_t)b) << " ";
    }
    std::cout << std::endl;
}

std::optional<std::size_t> Utils::getUserInputInt(const std::string& line, std::size_t max) {
    std::size_t ret;

    while (!std::cin.eof()) {
        std::cout << line << " (0-" << max << "): ";
        std::cin >> ret;

        if (!std::cin) {
            return {};
        }

        if (ret <= max) {
            return ret;
        }
    }
    return {};
}

std::optional<SimpleBLE::Adapter> Utils::getAdapter() {
    if (!SimpleBLE::Adapter::bluetooth_enabled()) {
        std::cout << "Bluetooth is not enabled!" << std::endl;
        return {};
    }

    auto adapter_list = SimpleBLE::Adapter::get_adapters();

    // no adapter found
    if (adapter_list.empty()) {
        std::cerr << "No adapter was found." << std::endl;
        return {};
    }

    // only one found, returning directly
    if (adapter_list.size() == 1) {
        auto adapter = adapter_list.at(0);
        std::cout << "Using adapter: " << adapter.identifier() << " [" << adapter.address() << "]" << std::endl;
        return adapter;
    }

    // multiple adapters found, ask user to select one
    std::cout << "Available adapters:" << std::endl;
    int i = 0;
    for (auto& adapter : adapter_list) {
        std::cout << "[" << i++ << "] " << adapter.identifier() << " [" << adapter.address() << "]" << std::endl;
    }

    auto adapter_selection = Utils::getUserInputInt("Select an adapter", adapter_list.size() - 1);

    if (!adapter_selection.has_value()) {
        return {};
    }

    return adapter_list[adapter_selection.value()];
}

SimpleBLE::Peripheral & select_and_connect_peripheral(std::vector<SimpleBLE::Peripheral>& peripherals) {
    while(1) {
        auto selection = Utils::getUserInputInt("Please select a device to connect to: ", peripherals.size() - 1);

        if (!selection.has_value()) {
            continue;
        }

        SimpleBLE::Peripheral & peripheral = peripherals[selection.value()];
        std::cout << "Connecting to " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        try {
            peripheral.connect();
            return peripheral;
        } catch (SimpleBLE::Exception::OperationFailed e) {
            std::cout << e.what() << std::endl;
        }
    }
}


static void format_table(tabulate::Table table) {
    table.format()
        .multi_byte_characters(true)
        .border_top(" ")
        .border_bottom(" ")
        .border_left(" ")
        .border_right(" ")
        .corner(" ")
        .font_align(tabulate::FontAlign::center)
        .hide_border_top();

    // Format header row
    table[0].format().padding_bottom(1).font_style({tabulate::FontStyle::bold});
}

tabulate::Table Utils::peripheral_table(std::vector<SimpleBLE::Peripheral> peripherals) {

    tabulate::Table table;
    table.add_row({"#", "Identifier", "Address", "Address Type", "Connectable", "RSSI", "TX Power", "#"});

    for (size_t i = 0; i < peripherals.size(); i++) {
        SimpleBLE::Peripheral & p = peripherals[i];
    
        std::string is_connectable = p.is_connectable() ? "true " : "false";
        std::string tx_power = p.tx_power() == -32768 ? "" : std::to_string(p.tx_power()) + " dB";;
        std::string rssi = std::to_string(p.rssi()) + " dB";

        table.add_row(tabulate::RowStream{} << i << p.identifier() << p.address()
        << p.address_type() << is_connectable << rssi << tx_power << i);
    }
    
    format_table(table);

    // Set identifier width
    table.column(1).format().width(30);

    return table;
}

tabulate::Table Utils::service_characteristics_table(SimpleBLE::Peripheral peripheral) {
    int i = 0;

    tabulate::Table table;
    table.add_row({"#", "Type", "UUID", "Read", "Write Request", "Write Command", "Notify", "Indicate", "#"});
    for (auto service : peripheral.services()) {
        
        table.add_row(tabulate::RowStream{} << "" << " S" << "" + service.uuid());
    
        for (auto ch : service.characteristics()) {
            table.add_row(tabulate::RowStream{} << std::boolalpha << i << " .C" << ch.uuid() <<
            ch.can_read() << ch.can_write_request() << ch.can_write_command() << ch.can_notify() << ch.can_indicate() << i);
            i++;

            for (auto& descriptor : ch.descriptors()) {
                table.add_row(tabulate::RowStream{} << "" << " ..D" << descriptor.uuid());            
            }
        }
    }

    format_table(table);
    table.column(1).format().font_align(tabulate::FontAlign::left);
    return table;
}