<?php
/**
 * Display a plain HTML VIEW ONLY version of the widget
 * with the specified rating
 *
 * @param $rating
 *   The desired rating to display out of 100 (i.e. 80 is 4 out of 5 stars)
 * @param $stars
 *   The total number of stars this rating is out of
 * @param $tag
 *   Allows multiple ratings per node
 * @return
 *   A themed HTML string representing the star widget
 *
 */
function theme_match_fivestar_static($rating, $stars = 5, $tag = 'vote') {
  drupal_add_css(drupal_get_path('module', 'match') .'/css/fivestar.css');
  
  $output = '';
  $output .= '<div class="fivestar-widget-static fivestar-widget-static-'. $tag .' fivestar-widget-static-'. $stars .' clear-block">';
  if (empty($stars)) {
    $stars = 5;
  }
  $numeric_rating = $rating/(100/$stars);
  for ($n=1; $n <= $stars; $n++) {
    $star_value = ceil((100/$stars) * $n);
    $prev_star_value = ceil((100/$stars) * ($n-1));
    $zebra = ($n % 2 == 0) ? 'even' : 'odd';
    $first = $n == 1 ? ' star-first' : '';
    $last = $n == $stars ? ' star-last' : '';
    $output .= '<div class="star star-'. $n .' star-'. $zebra . $first . $last .'">';
    if ($rating < $star_value && $rating > $prev_star_value) {
      $percent = (($rating - $prev_star_value) / ($star_value - $prev_star_value)) * 100;
      $output .= '<span class="on" style="width: '. $percent .'%">';
    }
    elseif ($rating >= $star_value) {
      $output .= '<span class="on">';
    }
    else {
      $output .= '<span class="off">';
    }
    if ($n == 1)$output .= $numeric_rating;
    $output .= '</span></div>';
  }
  $output .= '</div>';
  return $output;
}