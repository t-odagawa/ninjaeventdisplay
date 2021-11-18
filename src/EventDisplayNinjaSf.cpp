// system includes
#include <iomanip>
#include <fstream>
#include <ctime>

// boost includes
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

// Ninjasf includes
// #include <Sharing_file.hpp>
class Sharing_file {
public:
  int32_t pl, ecc_id, fixedwall_id, trackerwall_id, spotid, zone[2], rawid[2], unix_time, tracker_track_id, entry_in_daily_file, eventid, track_type;
  float chi2_shifter[3];
  // spotid:spotA * 100 + spotB
  // chi2_shifter : [0]:ECC-fixedwall [1]:fixedwall-TSS [2]:TSS-tracker
  static bool sort_unix_time(const Sharing_file &lhs, const Sharing_file &rhs) {
    if (lhs.unix_time == rhs.unix_time)return lhs.tracker_track_id < rhs.tracker_track_id;
    return lhs.unix_time < rhs.unix_time;
  }
  static bool sort_eventid(const Sharing_file &lhs, const Sharing_file &rhs) {
    if (lhs.eventid == rhs.eventid)return sort_unix_time(lhs, rhs);
    //lhs.unix_time < rhs.unix_time;
    return lhs.eventid < rhs.eventid;
  }
};

namespace logging = boost::log;

int main (int argc, char *argv[]) {

  logging::core::get()->set_filter
    (
     logging::trivial::severity >= logging::trivial::info
     );

  BOOST_LOG_TRIVIAL(info) << "=====Start!=====";

  if (argc != 3) {
    BOOST_LOG_TRIVIAL(info) << "Usage : " << argv[0]
			    << " <input ninjasf file path> <output txt file path>";
    std::exit(1);
  }

  try {

#ifdef TEXT_MODE
    std::ifstream ifs(argv[1]);
#else
    std::ifstream ifs(argv[1], std::ios::binary);
#endif
    Sharing_file t;

    std::ofstream ofs(argv[2]);

#ifdef TEXT_MODE
    int32_t tmp;
    float float_tmp;
    while (ifs >> t.pl >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp
	   >> t.unix_time >> tmp >> t.entry_in_daily_file 
	   >> float_tmp >> float_tmp >> float_tmp >> t.event_id >> tmp) {
#else
    while (ifs.read((char*)& t, sizeof(Sharing_file))) {
#endif

      time_t unixtime = (time_t)t.unix_time;
      tm *tm_event = localtime(&unixtime);

      int year = tm_event->tm_year + 1900;
      int month = tm_event->tm_mon + 1;
      int day = tm_event->tm_mday;
      BOOST_LOG_TRIVIAL(debug) << year << "-" << month << "-" << day;
      if (t.eventid == -1) continue;
      ofs << std::right << std::fixed
	  << std::setw(5) << std::setprecision(0) << year << " "
	  << std::setw(3) << std::setprecision(0) << month << " "
	  << std::setw(3) << std::setprecision(0) << day << " "
	  << std::setw(6) << std::setprecision(0) << t.entry_in_daily_file << " "
	  << std::setw(9) << std::setprecision(0) << t.eventid << " "
	  << std::endl;
      BOOST_LOG_TRIVIAL(debug) << "Corresponding entry : " << t.entry_in_daily_file;
    }


  } catch (const std::runtime_error &error) {
    BOOST_LOG_TRIVIAL(fatal) << "Runtime error : " << error.what();
    std::exit(1);
  } catch (const std::invalid_argument &error) {
    BOOST_LOG_TRIVIAL(fatal) << "Invalid argument error : " << error.what();
    std::exit(1);
  }

  BOOST_LOG_TRIVIAL(info) << "=====Finish!=====";
  std::exit(0);
}
