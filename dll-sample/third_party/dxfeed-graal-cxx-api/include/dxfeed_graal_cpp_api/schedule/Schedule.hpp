// Copyright (c) 2024 Devexperts LLC.
// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "../internal/Conf.hpp"

DXFCXX_DISABLE_MSC_WARNINGS_PUSH(4251)

DXFCPP_BEGIN_NAMESPACE

/**
 * <b>Schedule</b> class provides API to retrieve and explore trading schedules of different exchanges
 * and different classes of financial instruments. Each instance of schedule covers separate trading schedule
 * of some class of instruments, i.e. NYSE stock trading schedule or CME corn futures trading schedule.
 * Each schedule splits entire time scale into separate @ref Day "days" that are aligned to the specific
 * trading hours of covered trading schedule.
 */
struct DXFCPP_EXPORT Schedule {
    friend struct Day;

    /// The alias to a type of shared pointer to the Schedule object
    using Ptr = std::shared_ptr<Schedule>;

    /// The alias to a type of unique pointer to the Schedule object
    using Unique = std::unique_ptr<Schedule>;

  private:
    JavaObjectHandle<Schedule> handle_;

    explicit Schedule(void *handle = nullptr) noexcept;

    /**
     * Checks the handle, attempts to allocate memory for the pointer and return Schedule::Ptr
     *
     * @param handle The graal Schedule's handle
     * @return The smart pointer for the Schedule object.
     */
    static Schedule::Ptr create(void *handle);

  public:
    /**
     * Returns default schedule instance for specified instrument profile.
     *
     * @param profile The instrument profile those schedule is requested
     * @return The default schedule instance for specified instrument profile
     */
    static Schedule::Ptr getInstance(std::shared_ptr<InstrumentProfile> profile);

    /**
     * Returns default schedule instance for specified schedule definition.
     *
     * @param scheduleDefinition The schedule definition of requested schedule
     * @return The default schedule instance for specified schedule definition
     */
    static Schedule::Ptr getInstance(const std::string &scheduleDefinition);

    /**
     * Returns schedule instance for specified instrument profile and trading venue.
     *
     * @param profile The instrument profile those schedule is requested
     * @param venue The trading venue those schedule is requested
     * @return The schedule instance for specified instrument profile and trading venue
     */
    static Schedule::Ptr getInstance(std::shared_ptr<InstrumentProfile> profile, const std::string &venue);

    /**
     * Returns trading venues for specified instrument profile.
     *
     * @param profile The instrument profile those trading venues are requested
     * @return trading venues for specified instrument profile
     */
    static std::vector<std::string> getTradingVenues(std::shared_ptr<InstrumentProfile> profile);

    /**
     * Downloads defaults using specified download config and optionally start periodic download.
     * The specified config can be one of the following:<ul>
     * <li>"" - stop periodic download
     * <li>URL   - download once from specified URL and stop periodic download
     * <li>URL,period   - start periodic download from specified URL
     * <li>"auto"   - start periodic download from default location
     * </ul>
     *
     * @param downloadConfig download config
     */
    static void downloadDefaults(const std::string &downloadConfig) noexcept;

    /**
     * Sets shared defaults that are used by individual schedule instances.
     *
     * @param data The content of default data
     * @return `true` if OK
     */
    static bool setDefaults(const std::vector<char> &data) noexcept;

    /**
     * Returns session that contains specified time.
     * This method will return `Session::Ptr{nullptr}` if specified time
     * falls outside of valid date range from 0001-01-02 to 9999-12-30.
     *
     * @param time the time to search for
     * @return session that contains specified time or `Session::Ptr{nullptr}` if specified time
     * falls outside of valid date range from 0001-01-02 to 9999-12-30.
     */
    Session::Ptr getSessionByTime(std::int64_t time) const noexcept;

    /**
     * Returns day that contains specified time.
     * This method will return `Day::Ptr{nullptr}` (std::shared_ptr<Day>{nullptr}) if specified time
     * falls outside of valid date range from 0001-01-02 to 9999-12-30.
     *
     * @param time The time to search for
     * @return The day that contains specified time or `Day::Ptr{nullptr}` (std::shared_ptr<Day>{nullptr}) if
     * specified time falls outside of valid date range
     */
    Day::Ptr getDayByTime(std::int64_t time) const noexcept;

    /**
     * Returns day for specified day identifier.
     * This method will return `Day::Ptr{nullptr}` (std::shared_ptr<Day>{nullptr}) if specified day identifier
     * falls outside of valid date range from 0001-01-02 to 9999-12-30.
     *
     * @param dayId The day identifier to search for
     * @return The day for specified day identifier or `Day::Ptr{nullptr}` (std::shared_ptr<Day>{nullptr}) if
     * specified day identifier falls outside of valid date range
     * @see Day::getDayId()
     */
    Day::Ptr getDayById(std::int32_t dayId) const noexcept;

    /**
     * Returns day for specified year, month and day numbers.
     * Year, month, and day numbers shall be packed in the following way:
     * <pre>YearMonthDay = year * 10000 + month * 100 + day</pre>
     * For example, September 28, 1977 has value 19770928.
     * <p>
     * If specified day does not exist then this method returns day with
     * the lowest valid YearMonthDay that is greater than specified one.
     * This method will return `Day::Ptr{nullptr}` (std::shared_ptr<Day>{nullptr}) if specified year, month and day
     * numbers fall outside of valid date range from 0001-01-02 to 9999-12-30.
     *
     * @param yearMonthDay The year, month and day numbers to search for
     * @return The day for specified year, month and day numbers or `Day::Ptr{nullptr}` (std::shared_ptr<Day>{nullptr})
     * if specified year, month and day numbers fall outside of valid date range
     * @see Day#getYearMonthDay()
     */
    Day::Ptr getDayByYearMonthDay(std::int32_t yearMonthDay) const noexcept;

    /**
     * Returns session that is nearest to the specified time and that is accepted by specified filter.
     * This method will return `Session::Ptr{nullptr}` (std::shared_ptr<Session>{nullptr}) if specified time falls
     * outside of valid date range from 0001-01-02 to 9999-12-30 or if no sessions acceptable by specified filter are
     * found within one year this method
     * <p> To find nearest trading session of any type use this code:
     * <pre>session = schedule->getNearestSessionByTime(time, SessionFilter::TRADING);</pre>
     * To find nearest regular trading session use this code:
     * <pre>session = schedule->getNearestSessionByTime(time, SessionFilter::REGULAR);</pre>
     *
     * @param time The time to search for
     * @param filter the filter to test sessions
     * @return session that is nearest to the specified time and that is accepted by specified filter or
     * `Session::Ptr{nullptr}` (std::shared_ptr<Session>{nullptr}) if specified time falls outside of valid date range
     * or if no such day was found within one year
     */
    Session::Ptr getNearestSessionByTime(std::int64_t time, const SessionFilter &filter) const noexcept;

    /**
     * Returns session that is nearest to the specified time and that is accepted by specified filter.
     * This method will return `Session::Ptr{nullptr}` (std::shared_ptr<Session>{nullptr}) if specified time
     * falls outside of valid date range from 0001-01-02 to 9999-12-30.
     * If no sessions acceptable by specified filter are found within one year this method will return
     * `Session::Ptr{nullptr}` (std::shared_ptr<Session>{nullptr}).
     * <p>
     * To find nearest trading session of any type use this code:
     * <pre>session = schedule->findNearestSessionByTime(time, SessionFilter::TRADING);</pre>
     * To find nearest regular trading session use this code:
     * <pre>session = schedule->findNearestSessionByTime(time, SessionFilter::REGULAR);</pre>
     *
     * @param time The time to search for
     * @param filter The filter to test sessions
     * @return The session that is nearest to the specified time and that is accepted by specified filter or
     * `Session::Ptr{nullptr}` (std::shared_ptr<Session>{nullptr}) if specified time falls outside of valid date range
     */
    Session::Ptr findNearestSessionByTime(std::int64_t time, const SessionFilter &filter) const noexcept;

    /**
     * Returns name of this schedule.
     *
     * @return The name of this schedule
     */
    std::string getName() const noexcept;

    /**
     * Returns time zone display name in which this schedule is defined.
     *
     * @return time zone display name in which this schedule is defined
     */
    std::string getTimeZoneDisplayName() const noexcept;

    /**
     * Returns time zone id in which this schedule is defined.
     *
     * @return time zone id in which this schedule is defined
     */
    std::string getTimeZoneId() const noexcept;
};

DXFCPP_END_NAMESPACE

DXFCXX_DISABLE_MSC_WARNINGS_POP()