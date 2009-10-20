<?php
// $Id$

/**
 * @file:
 * Simple term extraction.
 */

/**
 * Finds tags in text.
 */
function extractor_simple_extract($text) {
  $tags = array();
  $text = strip_tags($text);
  $words = _extractor_simple_split($text);
  $vid = variable_get('extractor_simple_vid', 1);

  for ($pos = 0; $pos < count($words); $pos++) {
    $term_names = $term_tids = $terms = array();
    $word = $words[$pos];

    // 1) get all terms that start with this word.
    $result = db_query('SELECT tid, name FROM {term_data} WHERE vid = %d AND name RLIKE "^%s[[:>:]]"', $vid, $word); // If this query gets too slow, we need to build a word index.
    while ($term = db_fetch_object($result)) {
      $term_tids[$term->name] = $term->tid;
      $term_names[$term->name] = $term->name;
    }
    // Skip to immediate loop if no results.
    if (!count($term_tids)) {
      continue;
    }

    // 2) Order result by length desc.
    $sorted = FALSE;
    $sorted_terms = array_values($term_names);
    $terms = array();
    while (!$sorted) {
      $sorted = TRUE;
      for ($i = 0; $i < count($sorted_terms) - 1; $i++) {
        if (strlen($sorted_terms[$i]) < strlen($sorted_terms[$i+1])) {
          $tmp = $sorted_terms[$i+1];
          $sorted_terms[$i+1] = $sorted_terms[$i];
          $sorted_terms[$i] = $tmp;
          $sorted = FALSE;
        }
      }
    }
    foreach ($sorted_terms as $term) {
      $terms[$term_tids[$term]] = $term_names[$term];
    }

    // 4) Determine which tag fits by looking at next word in text.
    foreach ($terms as $tid => $term) {
      $term_words = _extractor_simple_split($term);
      // Iterate through tag words backwards, skip to the start of the outmost loop
      // if we find a mismatch.
      for ($i = count($term_words) - 1; $i >= 0; $i--) {
        if ($term_words[$i] != $words[$pos+$i]) {
          continue 2; // Skips to start of foreach ($terms as $term) {
        }
      }
      // We have a full match, add to result array, shift $pos pointer to the next word
      // after this term and break out of the immediate loop.
      $tags[$tid] = $term;
      $pos = $pos + count($term_words);
      break; // Skips to start of while (!$sorted) {
    }
  }

  return $tags;
}

/**
 * Split text into words.
 */
function _extractor_simple_split($text) {
  if (module_exists('search')) {
    $result = search_index_split(search_simplify($text));
  }
  else {
    $result = preg_split("/[\s,.:\-\(\)\[\]{}*\/]+/", $text);
  }
  // search_index_split can return NULL
  if ($result) {
    return array_filter($result);
  }
}


