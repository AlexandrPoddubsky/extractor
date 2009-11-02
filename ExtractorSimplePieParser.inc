<?php
// $Id$

/**
 * Extend simplepie parser and add term extraction.
 */
class ExtractorSimplePieParser extends FeedsSimplePieParser {

  /**
   * Add additional mapping sources.
   */
  public function getMappingSources() {
    $sources = array(
      'extracted_terms' => array(
        'name' => t('Extracted term names'),
        'description' => t('Array of term names extracted from title and or description.'),
      ),
      'extracted_tids' => array(
        'name' => t('Extracted term tids'),
        'description' => t('Array of term ids (tid) of term names extracted from title and or description.'),
      ),
    );
    return parent::getMappingSources() + $sources;
  }

  /**
   * If extracted_terms or extracted_tids was selected, extract terms and attach
   * them to item.
   */
  public function getSourceElement($item, $element_key) {
    if ($element_key == 'extracted_terms') {
      return extractor_extract($item['title'] .' '. $item['description']);
    }
    elseif ($element_key == 'extracted_tids') {
      $terms = extractor_extract($item['title'] .' '. $item['description']);
      return array_keys($terms);
    }
    else {
      return parent::getSourceElement($item, $element_key);
    }
  }
}
