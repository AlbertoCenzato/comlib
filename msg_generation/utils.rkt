#lang racket

(provide add-newline)
(provide add-semicolon)
(provide starts-uppercase?)


(define (add-semicolon str)
   (string-append str ";"))

(define (add-newline str)
   (string-append str "\n"))

(define (starts-uppercase? str)
      (regexp-match #rx"^[A-Z].*" str))
