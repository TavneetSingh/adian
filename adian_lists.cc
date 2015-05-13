#include "adian_list.h"

#define CURRENT_TIME Scheduler::instance().clock()

//---------------------------Failed Path List Implementation------------------------//

Adian_Failed_Path_list::Adian_Failed_Path_list() {}

void Adian_Failed_Path_list::add_new_packet(int uid, failed_path fp, double expire) {
	// It is already confirmed that entry for uid do not exit in the map
	
	// fp is not in list form
	pl_t newlist;
	newlist.push_back(fp);
	
	// insert path
	fl_[uid] = newlist;

	// insert expiry time
	el_[uid] = expire;
}

void Adian_Failed_Path_list::add_failed_path(int uid, nsaddr_t next_hop, nsaddr_t daddr) {

	// convert path to structure
	failed_path newpath;
	newpath.next_hop = next_hop;
	newpath.daddr = daddr;

	// First check if entry exists for uid
	fpl_t::iterator it = fl_.find(uid);

	if(it == fl_.end()) {
		// Entry does not exists
		add_new_packet(uid, newpath, CURRENT_TIME+FAILED_PATH_EXPIRE_TIME);
	}
	else {
		// Entry corrosponding to uid exists
		// push the path to the list of failed paths
		fl_[uid].push_back(newpath);
		// recently accessed, so add expiry time
		add_expire_time(uid, FAILED_PATH_EXPIRE_TIME);
	}
}

void Adian_Failed_Path_list::rm_entry(int uid) {
	fl_.erase(uid);
	el_.erase(uid);
}

int Adian_Failed_Path_list::check_failed_path(int uid, nsaddr_t next_hop, nsaddr_t daddr) {

	// first check if entry exists
	fpl_t::iterator it = fl_.find(uid);

	if(it == fl_.end()) {
		// entry does not exists
		return 0;	// false - entry do not exist
	}
	else {
		// entry for uid exists
		// now check if next_hop and daddr exists in the list
		pl_t::iterator list_it;
		for(list_it = fl_[uid].begin(); list_it != fl_[uid].end(); list_it++ ) {
			// iterate over whole list and check values
			if ((*list_it).next_hop == next_hop && (*list_it).daddr == daddr) {
				// path found
				return 1;
			}
		}
		return 0;
	}
}

double Adian_Failed_Path_list::expire_time(int uid) {
	// check if exists
	uid_expire_t::iterator it = el_.find(uid);

	if(it == el_.end()) {
		// uid not in list
		return 0.0;
	}
	else {
		return (*it).second;
	}
}

void Adian_Failed_Path_list::add_expire_time(int uid, double extra_time) {
	//find the entry
	uid_expire_t::iterator it = el_.find(uid);
	if(it == el_.end()) {
		// uid not in list
		return;
	}
	else {
		(*it).second += extra_time;
	}
}




//..............................Attempt List Implementation......................................//

Adian_Attempt_list::Adian_Attempt_list() {}

void Adian_Attempt_list::add_entry(int uid, int attempt, double expire) {
	
	
	//new attempt list
	al_t newlist;
	newlist.push_back(fp);
	
	// insert attempted path 
	al_[uid] = newlist;
	decrease_attempt(int);

	// insert expiry time
	el_[uid] = expire;
}


//remove entry for particular uid from lists.
void Adian_Attempt_list::rm_entry(int uid){
	al_.erase(uid);   //remove entry from attempt list corresponding to uid.
	el_.erase(uid);   //remove entry from expire list corresponding to uid.
 }


//reduce no. of attempts whenever traversed.
int Adian_Attempt_list::decrease_attempts(int uid){
	while(attempt>0){
        attempt_list_t::iterator it = al_.find(uid);
    
			for(it = al_[uid].begin(); it != fl_[uid].end();it++ ) {
			// iterate over whole list and check values
			if ((*it).next_hop == next_hop && (*it).daddr == daddr)
			attempt--;}
      }
	return(attempt);
}




double Adian_Attempt_list::expire_time(u_int32_t e_time){
	return(e_time);
}
	


//.....................Data Source List................................

Adian_Data_Source_list::Adian_Data_Source_list() {}

void Adian_Data_Source_list::add_data_source(int uid, nsaddr_t source, double expire){
	sl_ newpath;
	newpath.source = source;
	//newpath.daddr = daddr;
	//check if data source exists.
	src_list_t_t::iterator it = sl_.find(uid);

	if(it == sl_.end()) {
		// Entry does not exists
		add_data_source(uid, newpath, CURRENT_TIME+FAILED_PATH_EXPIRE_TIME);
	}
	else {
		// Entry corrosponding to uid exists
		// push the path to the list of failed paths
		sl_[uid].push_back(newpath);
	}
}


void Adian_Data_Source_list::rm_data_source(int uid){
	sl_erase(uid);
        el_erase(uid);
}


double  Adian_Data_Source_list::expire_time(u_int32_t expire){
	return( CURRENT_TIME+expire);
}



//-----------------------------Reply Route List--------------------------------//
Adian_Reply_Route_list::Adian_Reply_Route_list() {}

void Adian_Reply_Route_list::add_reply_route(u_int32_t seq_num_, nsaddr_t reply_to, double expire){
	rl_ newpath;
	newpath.reply_to = reply_to;
	newpath.expire=expire;
}


void Adian_Reply_Route_list::rm_reply_route(u_int32_t){
	rl_erase(seq_num_);
	el_erase(seq_num_);
}


double Adian_Reply_Route_list::expire_time(u_int32_t expire){
	return(CURRENT_TIME+expire);
}

