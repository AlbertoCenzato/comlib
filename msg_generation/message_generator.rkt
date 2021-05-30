#lang racket

(require scribble/text)
(require racket/file)
(require "codegeneration.rkt")

(define (split-on-whitespace str)
  (string-split str " "))

(define (generate-code-from-msg-file inputFileName outputDir)
  (let* ([lines (file->lines inputFileName)]
         [className (first lines)]
         [membersLines (rest lines)]
         [members (map parse-field-declaration membersLines)]
         [headerFileName (string-append-immutable className ".h")]
         [sourceFileName (string-append-immutable className ".cpp")]
         [headerOutputPath (build-path outputDir headerFileName)]
         [sourceOutputPath (build-path outputDir sourceFileName)])

    (define (write-file out code-generating-function)
      (define sourceCode (code-generating-function className members))
      (output sourceCode out))

    (define (write-header-file out)
      (write-file out generate-header))

    (define (write-source-file out)
      (write-file out generate-source))

    (when (regexp-match #rx"^[a-z].*" className)
      (error (string-append-immutable "User defined type " className " must start with an uppercase character!")))

    (unless (directory-exists? outputDir)
      (make-directory outputDir))
    (call-with-output-file headerOutputPath write-header-file #:exists 'replace)
    (call-with-output-file sourceOutputPath write-source-file #:exists 'replace)))

(define input-msg-file (make-parameter ""))
(define output-directory (make-parameter ""))
(command-line #:args (input output) (input-msg-file input) (output-directory output))
(generate-code-from-msg-file (input-msg-file) (output-directory))