<?php
// $Id$

/**
 * Extend common syndication parser and add term extraction.
 */
class ExtractorSyndicationParser extends FeedsSyndicationParser {

  /**
   * Add additional mapping sources.
   */
  public function getMappingSources() {
    $sources = array(
      'extracted_terms' => t('Extracted term names'),
      'extracted_tids' => t('Extracted term tids'),
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
