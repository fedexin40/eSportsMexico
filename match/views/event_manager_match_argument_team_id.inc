<?php
class event_manager_match_argument_team_id extends views_handler_argument_numeric{
  function query(){
    //we need to make sure that the fields we need for the points difference are added to the query
    $this->ensure_my_table();
    $group = $this->query->set_where_group('OR', NULL, 'where');
    $this->query->add_where($group, '{event_manager_match}.home_team = '.$this->get_value());
    $this->query->add_where($group, '{event_manager_match}.away_team = '.$this->get_value());
  }
}