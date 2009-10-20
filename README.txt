$Id$

EXTRACTOR
=========

Simple term extraction API.

Usage
=====

With Feeds module:

- Install module
- Edit a feeds configuration, pick either "Common syndication parser with term
  extraction" or "SimplePie parser with term extraction".
- Go to "Mapping" settings of the processor and pick "Extracted term names" or
  "Extracted term tids" from the source drop down and map it to any target that
  handles arrays.

As API:

$terms = extractor_extract($text);
