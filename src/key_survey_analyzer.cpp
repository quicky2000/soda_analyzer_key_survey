/*
  This file is part of osm_diff_analyzer_key_survey, Openstreetmap
  diff analyzer based on CPP diff representation. It's aim is to survey
  editions concerning a specified tag
  Copyright (C) 2013  Julien Thevenon ( julien_thevenon at yahoo.fr )

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "key_survey_analyzer.h"
#include "key_survey_common_api.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cassert>

namespace osm_diff_analyzer_key_survey
{
  //------------------------------------------------------------------------------
  key_survey_analyzer::key_survey_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf,
                                           key_survey_common_api & p_api):
    osm_diff_analyzer_cpp_if::cpp_analyzer_base("key_survey_analyser",p_conf->get_name(),""),
    m_api(p_api),
    m_report(),
    m_searched_string(""),
    m_first_diff(true),
    m_first_timestamp(""),
    m_last_timestamp("")
                                          //TO DELETE    m_db(get_name()+".sqlite3")
  {
    // Register module to be able to use User Interface
    m_api.ui_register_module(*this,get_name());

    const std::map<std::string,std::string> & l_conf_parameters = p_conf->get_parameters();

    // Searched string parameter management
    std::map<std::string,std::string>::const_iterator l_iter = l_conf_parameters.find("searched_string");
    if(l_iter == l_conf_parameters.end())
      {
	std::cout << "ERROR : missing mandatory \"searched_string\" parameter in module \"" << get_name() <<"\"" << std::endl ;
	exit(-1);
      }

    {
      std::stringstream l_stream;
      l_stream << "parameter[\"searched_string\"]=\"" << l_iter->second << "\"";
      m_api.ui_append_log_text(*this,l_stream.str());
    }
    m_searched_string = l_iter->second;
  }

  //------------------------------------------------------------------------------
  void key_survey_analyzer::create_report(void)
  {
    std::string l_report_file_name = m_searched_string+"_detailled_object_report";
    std::string l_summary_report_file_name = m_searched_string+"_object_report";
    std::string l_complete_report_file_name = l_report_file_name + ".html";
    std::string l_complete_summary_report_file_name = l_summary_report_file_name + ".html";
    std::ifstream l_test_file;
    uint32_t l_number = 0;
    bool l_continu = true;
    while(l_continu)
      {
        l_test_file.open(l_complete_report_file_name.c_str());
        l_continu = l_test_file.is_open();
        if(l_continu)
          {
            ++l_number;
            std::stringstream l_number_str;
            l_number_str << l_number;
            l_complete_report_file_name = l_report_file_name + "_" + l_number_str.str() + ".html";
            l_complete_summary_report_file_name = l_summary_report_file_name + "_" + l_number_str.str() + ".html";
            l_test_file.close();
          }
      }

    
    // Creating report
    m_report.open(l_complete_report_file_name.c_str());
    if(m_report.fail())
      {
	std::cout << "ERROR : unabled to open \"" << l_complete_report_file_name << "\"" << std::endl ;
	exit(EXIT_FAILURE);
      }
    m_report << "<html>" << std::endl ;
    m_report << "\t<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << std::endl ;
    m_report << "\t\t<title>" << m_searched_string << " tag survey detailled report</title>" << std::endl ;
    m_report << "\t</head>" << std::endl ;
    m_report << "\t<body><H1>" << m_searched_string << " tag survey detailled report</H1>" << std::endl ;
    m_report << "<H3>Beginning of analyze : " << m_first_timestamp << "</H3>" << std::endl ;

    // Creating report
    m_summary_report.open(l_complete_summary_report_file_name.c_str());
    if(m_summary_report.fail())
      {
	std::cout << "ERROR : unabled to open \"" << l_complete_summary_report_file_name << "\"" << std::endl ;
	exit(EXIT_FAILURE);
      }
    m_summary_report << "<html>" << std::endl ;
    m_summary_report << "\t<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">" << std::endl ;
    m_summary_report << "\t\t<title>" << m_searched_string << " tag survey report</title>" << std::endl ;
    m_summary_report << "\t</head>" << std::endl ;
    m_summary_report << "\t<body><H1>" << m_searched_string << " tag survey report</H1>" << std::endl ;

    m_api.ui_declare_html_report(*this,l_complete_report_file_name);
  }

  //------------------------------------------------------------------------------
  key_survey_analyzer::~key_survey_analyzer(void)
  {
    if(m_report.is_open())
      {
        m_report << "<H3>End of analyze : " << m_last_timestamp << "</H3>" << std::endl ;
        m_report << "</body>" << std::endl ;
        m_report << "</html>" << std::endl ;        
        m_report.close();
      }
    if(m_summary_report.is_open())
      {
        m_summary_report << "<HR>" << std::endl ;
        m_summary_report << "<H1>Statistics on edits from " << m_first_timestamp << " to " << m_last_timestamp << "</H1><BR>" << std::endl ;
        m_summary_report << "<ul>" << std::endl ;
        m_summary_report << "<li>Object created : " << m_stats.get_object_creation() << "</li>" << std::endl ;
        m_summary_report << "<li>Objects removed : " << m_stats.get_object_removed() << "</li>" << std::endl ;
        m_summary_report << "<li>Objects modified : " << m_stats.get_object_modified() << "</li>" << std::endl ;
        m_summary_report << "<li>Tags added : " << m_stats.get_tag_added() << "</li>" << std::endl ;
        m_summary_report << "<li>Tags modified : " << m_stats.get_tag_modified() << "</li>" << std::endl ;
        m_summary_report << "<li>Tags removed : " << m_stats.get_tag_removed() << "</li>" << std::endl ;
        m_summary_report << "<li>Involved people : " << m_stats.get_user_stats().size() << "</li>" << std::endl ;
        m_summary_report << "</ul>" << std::endl ;

        std::multimap<uint32_t,std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string> > l_user_rank;
	std::map<std::string,std::pair<osm_api_data_types::osm_object::t_osm_id,uint32_t> > l_user_list;

	//Fill user list and user rank
        const std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t>  & l_user_stats = m_stats.get_user_stats();
        for(std::map<std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string>,uint32_t>::const_iterator l_iter = l_user_stats.begin();
            l_iter != l_user_stats.end();
            ++l_iter)
          {
	    l_user_list.insert(std::make_pair(l_iter->first.second,std::make_pair(l_iter->first.first,l_iter->second)));
	    l_user_rank.insert(std::make_pair(l_iter->second,std::make_pair(l_iter->first.first,l_iter->first.second)));
	  }
	
	// Displau user stats sorted by user name
        m_summary_report << "<H2>User statistics</H2><BR>" << std::endl ;
        m_summary_report << "<TABLE BORDER=\"1\">" << std::endl ;
        m_summary_report << "<TR><TH>OSM Contributor</TH><TH>Nb edits</TH></TR>" << std::endl ;
	for(std::map<std::string,std::pair<osm_api_data_types::osm_object::t_osm_id ,uint32_t> >::const_iterator l_iter = l_user_list.begin();
	    l_iter != l_user_list.end();
	    ++l_iter)
	  {
            m_summary_report << "<TR><TD>";
            display_user_html_link(m_summary_report,l_iter->first,l_iter->second.first);
	    m_summary_report << "</TD><TD>" << l_iter->second.second << "</TD></TR>" << std::endl ;  
	  }
        m_summary_report << "</TABLE>" << std::endl ;

	// Display user stats sorted by modification number
        m_summary_report << "<H2>User rank</H2><BR>" << std::endl ;
        m_summary_report << "<TABLE BORDER=\"1\">" << std::endl ;
        m_summary_report << "<TR><TH>OSM COntributor</TH><TH>Nb edits</TH></TR>" << std::endl ;
        for(std::multimap<uint32_t,std::pair<osm_api_data_types::osm_object::t_osm_id ,std::string> >::const_reverse_iterator l_iter = l_user_rank.rbegin();
            l_iter != l_user_rank.rend();
            ++l_iter)
          {
            m_summary_report << "<TR><TD>";
            display_user_html_link(m_summary_report,l_iter->second.second,l_iter->second.first);
            m_summary_report << "</TD><TD>" << l_iter->first << "</TD></TR>" << std::endl ;
          }
         
        m_summary_report << "</TABLE>" << std::endl ;
        m_summary_report << "</body>" << std::endl ;
        m_summary_report << "</html>" << std::endl ;
        m_summary_report.close();
      }
  }

  //------------------------------------------------------------------------------
  void key_survey_analyzer::init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state)
  {
    std::stringstream l_stream;
    l_stream << "Starting analyze of diff " << p_diff_state->get_sequence_number() << std::endl ;
    m_api.ui_append_log_text(*this,l_stream.str());

    m_last_timestamp = p_diff_state->get_timestamp();
    if(m_first_diff)
      {
        m_first_timestamp = p_diff_state->get_timestamp();
        m_first_diff = false;
      }
  }

  //------------------------------------------------------------------------------
  void key_survey_analyzer::analyze(const std::vector<osm_api_data_types::osm_change*> & p_changes)
  {
    for(std::vector<osm_api_data_types::osm_change*>::const_iterator l_iter = p_changes.begin();
        l_iter != p_changes.end();
        ++l_iter)
      {
        const osm_api_data_types::osm_core_element * const l_element = (*l_iter)->get_core_element();
        assert(l_element);

        switch(l_element->get_core_type())
          {
          case osm_api_data_types::osm_core_element::NODE :
	    generic_analyze<osm_api_data_types::osm_node>(l_element,(*l_iter)->get_type());
            break;
          case osm_api_data_types::osm_core_element::WAY :
	    generic_analyze<osm_api_data_types::osm_way>(l_element,(*l_iter)->get_type());
            break;
          case osm_api_data_types::osm_core_element::RELATION :
	    generic_analyze<osm_api_data_types::osm_relation>(l_element,(*l_iter)->get_type());
            break;
          case osm_api_data_types::osm_core_element::INTERNAL_INVALID:
            std::cout << "ERROR : unexpected core type value \"" << osm_api_data_types::osm_core_element::get_osm_type_str(l_element->get_core_type()) << "\"" << std::endl ;
            exit(-1);
            break;
          }

      }
  }

  //------------------------------------------------------------------------------
  const std::string & key_survey_analyzer::get_input_type(void)const
  {
    return m_description.get_input_type();
  }

  //------------------------------------------------------------------------------
  const std::string & key_survey_analyzer::get_output_type(void)const
  {
    return m_description.get_output_type();
  }

  //------------------------------------------------------------------------------
  const std::string & key_survey_analyzer::get_type(void)const
  {
    return m_description.get_type();
  }

  key_survey_analyzer_description key_survey_analyzer::m_description;
}
//EOF
