#lang raket

(provide add-newline)
(provide add-semicolon)
(provide starts-uppercase?)


(define (add-semicolon str)
   (string-append-immutable str ";"))

(define (add-newline str)
   (string-append-immutable str "\n"))

(define (starts-uppercase? str)
      (regexp-match #rx"^[A-Z].*" str))
