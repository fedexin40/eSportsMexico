<?php

/**
 * @file
 * Default theme implementation to present an picture configured for the
 * teams's logo.
 *
 * Available variables:
 * - $picture: Image set by the user or the site's default. Will use
 * - $imagecache_used: TRUE if imagecache was used to size the picture. This
 *   tells us if we want to link to the full sized image.
 *
 */
?>

<?php if (!empty($picture)): ?>
  <div class="team-pane">
    <div class="picture">
        <?php print $picture; ?>
    </div>
  </div>
<?php endif; ?>

