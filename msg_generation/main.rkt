#lang racket

(require racket/cmdline)
(require "codegeneration.rkt")

; ------------------ script execution ------------------

(define (split-on-whitespace str)
  (string-split str " "))

(define (get-file-content inputFileName outputDir)
  (let* ([lines (file->lines inputFileName)]
         [className (first lines)]
         [membersLines (rest lines)]
         [members (map split-on-whitespace membersLines)]
         [headerFileName (string-append-immutable className ".h")]
         [sourceFileName (string-append-immutable className ".cpp")])

    (define (write-header-file out)
      (define (print-ext str)
        (display str out))
      (print-header print-ext className members))

    (define (write-source-file out)
      (define (print-ext str)
        (display str out))
      (print-source print-ext className members))

    (when (not (directory-exists? outputDir))
      (make-directory outputDir))
    (call-with-output-file (build-path outputDir headerFileName) write-header-file #:exists 'replace)
    (call-with-output-file (build-path outputDir sourceFileName) write-source-file #:exists 'replace)))

(define args
  (command-line #:args (filename output) (list filename output)))

(apply get-file-content args)
