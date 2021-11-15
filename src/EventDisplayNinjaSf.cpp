// system includes
#include <fstream>
#include <ctime>

// boost includes
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

// B2 includes
#include <B2Reader.hh>
#include <B2SpillSummary.hh>
#include <B2BeamSummary.hh>

// Ninjasf includes
// #include <Sharing_file.hpp>
class Sharing_file {
public :
  int32_t pl, ecc_id, fixedwall_id, trackerwall_id, spotid, zone[2], rawid[2], unix_time, tracker_track_id;
  int32_t entry_in_daily_file, event_id, track_type;
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
    while (ifs >> t.pl >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp
	   >> t.unix_time >> tmp >> t.entry_in_daily_file >> t.event_id >> tmp) {
#else
    while (ifs.read((char*)& t, sizeof(Sharing_file))) {
#endif

      time_t unixtime = (time_t)t.unix_time;
      tm *tm_event = localtime(&unixtime);

      int year = tm_event->tm_year + 1900;
      int month = tm_event->tm_mon + 1;
      int day = tm_event->tm_mday;
      BOOST_LOG_TRIVIAL(debug) << year << "-" << month << "-" << day;

      ofs << std::right << std::fixed
	  << std::setw(5) << std::setprecision(0) << year << " "
	  << std::setw(3) << std::setprecision(0) << month << " "
	  << std::setw(3) << std::setprecision(0) << day << " "
	  << std::setw(6) << std::setprecision(0) << t.entry_in_daily_file << " "
	//<< std::setw(9) << std::setprecision(0) << t.group_id << " "
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
