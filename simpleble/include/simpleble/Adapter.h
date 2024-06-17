#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <simpleble/export.h>
#include <simpleble/Exceptions.h>
#include <simpleble/Peripheral.h>
#include <simpleble/Types.h>

namespace SimpleBLE {

class AdapterBase;

/**
 * @brief Represents a Bluetooth Low Energy adapter.
 * 
 * This class provides methods to interact with Bluetooth Low Energy (BLE) adapters, 
 * including scanning for peripherals and handling connection callbacks.
 */
class SIMPLEBLE_EXPORT Adapter {
  public:
    /**
     * @brief Default constructor for the Adapter class.
     */
    Adapter() = default;

    /**
     * @brief Virtual destructor for the Adapter class.
     */
    virtual ~Adapter() = default;

    /**
     * @brief Checks if the adapter is initialized.
     * 
     * @return true if the adapter is initialized, false otherwise.
     * 
     * @note This method should be called before performing any operations on the adapter.
     */
    bool initialized() const;

    /**
     * @brief Retrieves the underlying implementation-specific handle of the adapter.
     * 
     * @return void* Pointer to the underlying adapter handle.
     * 
     * @warning Directly manipulating the underlying handle may cause undefined behavior.
     */
    void* underlying() const;

    /**
     * @brief Gets the identifier of the adapter.
     * 
     * @return std::string The identifier of the adapter.
     * 
     * @example
     * @code
     * Adapter adapter = Adapter::get_adapters().front();
     * std::string id = adapter.identifier();
     * std::cout << "Adapter Identifier: " << id << std::endl;
     * @endcode
     */
    std::string identifier();

    /**
     * @brief Gets the Bluetooth address of the adapter.
     * 
     * @return BluetoothAddress The Bluetooth address of the adapter.
     */
    BluetoothAddress address();

    /**
     * @brief Starts asynchronous scan for peripherals.
     * 
     * This method initiates a scan for BLE peripherals. The scan results can be retrieved
     * using the scan_get_results() method or through the scan callbacks.
     */
    void scan_start();

    /**
     * @brief Stops asynchronous scan for peripherals.
     * 
     * This method stops an ongoing scan for BLE peripherals.
     */
    void scan_stop();

    /**
     * @brief Scans for peripherals for a specified duration.
     * 
     * @param timeout_ms Duration of the scan in milliseconds.
     * 
     * @post The scan results can be retrieved using scan_get_results().
     * @note This is a blocking method.
     */
    void scan_for(int timeout_ms);

    /**
     * @brief Checks if the scan is currently active.
     * 
     * @return true if the scan is active, false otherwise.
     */
    bool scan_is_active();

    /**
     * @brief Retrieves the results of the scan.
     * 
     * @return std::vector<Peripheral> List of found peripherals.
     */
    std::vector<Peripheral> scan_get_results();

    /**
     * @brief Sets a callback function to be called when the scan starts.
     * 
     * @param on_scan_start The callback function.
     * 
     * @note The callback function should take no parameters and return void.
     */
    void set_callback_on_scan_start(std::function<void()> on_scan_start);

    /**
     * @brief Sets a callback function to be called when the scan stops.
     * 
     * @param on_scan_stop The callback function.
     * 
     * @note The callback function should take no parameters and return void.
     */
    void set_callback_on_scan_stop(std::function<void()> on_scan_stop);

    /**
     * @brief Sets a callback function to be called when the scan results are updated.
     * 
     * @param on_scan_updated The callback function.
     * 
     * @note The callback function should take a Peripheral and return void.
     */
    void set_callback_on_scan_updated(std::function<void(Peripheral)> on_scan_updated);

    /**
     * @brief Sets a callback function to be called when a new peripheral is found during the scan.
     * 
     * @param on_scan_found The callback function.
     * 
     * @note The callback function should take a Peripheral and return void.
     */
    void set_callback_on_scan_found(std::function<void(Peripheral)> on_scan_found);

    /**
     * @brief Gets a list of peripherals that are currently paired with the adapter.
     * 
     * @return std::vector<Peripheral> List of paired peripherals.
     * 
     */
    std::vector<Peripheral> get_paired_peripherals();

    /**
     * @brief Checks if Bluetooth is enabled on the system.
     * 
     * @return true if Bluetooth is enabled, false otherwise.
     */
    static bool bluetooth_enabled();

    /**
     * @brief Fetches a list of all available adapters.
     * 
     * @return std::vector<Adapter> List of available adapters.
     * 
     * @note This is a static method that returns all adapters available on the system.
     */
    static std::vector<Adapter> get_adapters();

  protected:
    std::shared_ptr<AdapterBase> internal_; /**< Internal implementation-specific data. */
};

}  // namespace SimpleBLE
