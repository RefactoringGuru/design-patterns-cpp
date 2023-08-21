/**
 * EN: Real World Example of the Singleton Design Pattern
 *
 * Need: Consider a (large) program that must implement its own internal logging
 * functionality with a global logger object. Suppose that all log messages are
 * required to be printed in order even if the logger is called across multiple
 * concurrent threads or processes. Furthermore, the logger should have some
 * sort of flag to specify and ignore messages below a certain level.
 *
 * Solution: A thread-safe Logger class can be implemented using the Scott
 * Meyers' Singleton pattern. The Singleton pattern is the recommended solution
 * if indeed there must be a single global instance of the Logger class.
 * However, in modern practices, the addition of a new singleton to a codebase
 * could be regarded as a design flaw with the singleton itself being a design
 * anti-pattern. Nevertheless, the following presents a Logger Singleton as a
 * commonly appearing use case of the pattern in the C++ literature.
 */

#include <iostream>
#include <mutex>
#include <string>
#include <thread>

/**
 * EN: The Logger Singleton Class
 *
 * In this (zero handle objects) implementation of the Scott Meyers' Singleton,
 * the constructor and destructor are private methods and the move/copy
 * constructors and assignment operations are explicitly deleted. In essence,
 * the program itself cannot directly create an instance of the Logger class,
 * and instead the static instance() member function must be used to access it.
 *
 * The public API of this Logger has two main callbacks: (1) set the level of
 * the Logger; and (2) log a message at given level. For convenience, these two
 * client-facing methods wrap around the instance() member function in a
 * thread-safe fashion. An integral counter member is also included to
 * demonstrate that the message ordering is preserved.
 *
 * Note the final keyword specifier prevents inheritance, that is, it is not
 * possible to extend this Logger Singleton and override its class methods.
 */

class Logger final {
public:
  /**
   * EN: Various levels for the log messages can be labelled here; the choice of
   * the level member establishes a threshold below which log messages are
   * ignored.
   */
  enum class Level : unsigned {
    debug = 0,
    info = 1,
    warning = 2,
    error = 3,
    /* ... */
  };

public:
  /**
   * EN: The Public API of this Logger
   *
   * Note that both of these methods must be implemented in a thread-safe
   * manner, hence the mutex as a static member.
   */
  static void level(Level);
  static void log(std::string const &, Level level = Level::debug);

public:
  /**
   * EN: Prevention of Copy and Move Construction
   */
  Logger(Logger const &) = delete;
  Logger(Logger &&) = delete;

  /**
   * EN: Prevention of Copy and Move Assigment Operations
   */
  Logger &operator=(Logger const &) = delete;
  Logger &operator=(Logger &&) = delete;

  /**
   * EN: Public Instantiator Method
   *
   * In a typical Singleton, this static member function would enable access to
   * the Singleton. In this implementation of a Logger class, it is called
   * inside of the bodies of the public API methods.
   */
  static Logger &instance();

private:
  /**
   * EN: Private Constructor and Destructor
   */
  Logger();
  ~Logger();

private:
  static std::mutex mutex_;
  static std::ostream &os_;
  static std::size_t count_;
  static Level level_;
};

/**
 * EN: Static members of the Logger class need to be defined outside of the
 * class itself.
 */
std::mutex Logger::mutex_;
std::ostream &Logger::os_{std::cout};
std::size_t Logger::count_{0};
Logger::Level Logger::level_{Logger::Level::debug};

/**
 * EN: Magic Static (c.f. Scott Meyers' Singleton)
 *
 * The instance() method creates a local static instance of the Logger class,
 * which is guaranteed thread-safe initialisation without manual thread
 * synchronisation. Note that this does not guarantee the thread safety of other
 * members; the RAII (Resource Acquistion Is Initialisation) principle should be
 * used to lock and unlock the mutex.
 *
 * Note that there will be a performance penalty each time this method is
 * called as there will be a check to see if the instance has already been
 * initialised.
 */
Logger &Logger::instance() {
  static Logger instance;
  return instance;
}

/**
 * EN: Logger Level Modifier Method
 *
 * This thread-safe setter allows the client to alter the (global) level member
 * of the Logger.
 */

void Logger::level(Logger::Level level) {
  std::lock_guard<std::mutex> lock(mutex_);
  instance().level_ = level;
}

/**
 * EN: Enummeration-to-String Helper Function
 *
 * This implementation is naive but nonetheless useful for distinguishing the
 * different kinds of log messages.
 */
std::string to_string(Logger::Level level) {
  switch (level) {
  case Logger::Level::debug:
    return "[DEBUG]";
  case Logger::Level::info:
    return "[INFO]";
  case Logger::Level::warning:
    return "[WARNING]";
  case Logger::Level::error:
    return "[ERROR]";
    /* ... */
  default:
    return "[LEVEL]";
  }
};

/**
 * EN: Thread-Safe Log Method
 *
 * If the message level is at or above the threshold level of the Logger
 * Singleton, then the counter is incremented and the message is printed.
 * Otherwise, the message is ignored and the counter remains as is.
 *
 * Note again the usage of RAII for mutex locking/unlocking should this method
 * be called in a thread.
 */
void Logger::log(std::string const &message, Logger::Level level) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (static_cast<int>(level) < static_cast<int>(instance().level_))
    return;
  instance().os_ << ++instance().count_ << '\t' << to_string(level) << "\n\t"
                 << message << '\n';
}

/**
 * EN: Constructor and Destructor
 *
 * The print statements indicate when these methods are called in the program.
 */
Logger::Logger() { std::cout << "****\tLOGGER\tSTART UP\t****" << '\n'; }
Logger::~Logger() { std::cout << "****\tLOGGER\tSHUT DOWN\t****" << std::endl; }

/**
 * EN: Client Code: Logger Singleton Usage
 *
 * The desired Log Level is set which also instantiates the Logger class; the
 * log() methods can then be invoked e.g. via lambdas within different threads.
 */
int main() {

  std::cout << "//// Logger Singleton ////\n";

  Logger::level(Logger::Level::info);

  std::thread t1(
      [] { Logger::log("This is just a simple development check."); });
  std::thread t2(
      [] { Logger::log("Here are some extra details.", Logger::Level::info); });
  std::thread t3([] {
    Logger::log("Be careful with this potential issue.",
                Logger::Level::warning);
  });
  std::thread t4([] {
    Logger::log("A major problem has caused a fatal stoppage.",
                Logger::Level::error);
  });

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  return EXIT_SUCCESS;
}