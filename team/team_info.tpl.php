<?php

/**
 * @file
 * Default theme implementation to present the teams's info.
 *
 * Available variables:
 * - $tid: Image set by the user or the site's default. Will use
 * - $ranking: TRUE if imagecache was used to size the picture. This
 * - $division: TRUE if imagecache was used to size the picture. This
 * - $calificacion:
 *   tells us if we want to link to the full sized image.
 *
 */
?>

<div class = "fondo_juegos">
  <?php print '<div class = "left_'. str_replace( " ", "_", $modalidad->Nombre) .'"></div>'; ?>
  <div class = "datos_equipo">
      <ul>
          <?php print '<li>'. l($modalidad->Nombre, 'taxonomy/term/'. $modalidad->tid) .'</li>'; ?>
          <?php print '<li>'. l($team->title, 'node/'. $team->nid) .'</li>';  ?>
          <?php print '<li>'. t('Ranking: '). $ranking .'</li>'; ?>
          <?php print '<li>'. $calificacion .'</li>'; ?>
      </ul>
  </div>
  <div class= "division">
    <?php print '<span class = "'. str_replace( " ", "_", $division) .'>'. str_replace( " ", "_", $division) .'</span>'; ?>
  </div>
  <?php print '<div class = "right_'. str_replace( " ", "_", $modalidad->Nombre) .'"></div>'; ?>
</div>

