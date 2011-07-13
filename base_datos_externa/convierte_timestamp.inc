<?php


	/**
	 *	Convierte timestamp de y a tipo date
	 */
	function convierte_a_timestamp( $fecha )
	{
		
		$user_timezone_name = date_default_timezone_name( TRUE );
		//drupal_set_message('user timezone name = '. $user_timezone_name);

		//$text_date = '2010-02-14';
		
		//drupal_set_message('start date = '. $fecha );

		// Convert the user entered date into a PHP 5 DateTime object
		$local_date = new DateTime( $fecha );

		// Reformat the user entered date into an ISO date that date_make_date() will accept
		$iso_date_string = date_format_date($local_date, 'custom', 'Y-m-d');
		//drupal_set_message('iso date_string = '. $iso_date_string);

		// Create a DateTime object with the user's timezone info
		$utc_date = date_make_date($iso_date_string, $user_timezone_name, DATE_DATETIME );

		// Change the timezone to UTC
		date_timezone_set($utc_date, timezone_open('UTC'));

		// Format the UTC version of the DateTime for use in node->save()
		$utc_date_string = date_format_date($utc_date, 'custom', 'Y-m-d');
		//drupal_set_message('utc date string = '. $utc_date_string);

		// convert to timestamp
		$timestamp = strtotime( $utc_date_string );
		//drupal_set_message('timestamp = '. $timestamp );

		return $timestamp;
		
	}
	
	
	