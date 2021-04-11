#lang racket

(require scribble/text)

(provide generate-header)
(provide generate-source)

(define (generate-header className members)
  (let ([memberVariables (declare-members members)]
        [constructorParams (generate-members-as-params members)]
        [constructorParamsInit (generate-constructor-params-init members)]
        [sizeof (generate-sizeof members)]
        [serialization (generate-serialization members)]
        [deserialization (generate-deserialization members)])
    (include/text "template.h")))

(define (generate-source className members)
  (let ([constructorParams (generate-members-as-params members)]
        [constructorParamsInit (generate-constructor-params-init members)]
        [sizeof (generate-sizeof members)]
        [serialization (generate-serialization members)]
        [deserialization (generate-deserialization members)])
    (include/text "template.cpp")))

(define (id member)
   (second member))

(define (type member)
   (first member))

(define (add-semicolon str)
   (string-append-immutable str ";"))

(define (add-newline str)
   (string-append-immutable str "\n"))

(define (generate-type-id-list members)
   (map string-join members))

(define (declare-members class-members)
   (define (transform str)
      (add-newline (add-semicolon str)))
   (map transform (generate-type-id-list class-members)))

(define (generate-members-as-params members)
   (string-join (generate-type-id-list members) ", "))

(define (generate-constructor-params-init members)
   (define (generate-member-initialization member)
      (string-append-immutable (id member) "(" (id member) ")"))
   (string-join (map generate-member-initialization members) ", "))

(define (generate-serialization members)
   (define (generate-field-serialization member)
      (string-append-immutable "buffer = com::serialize(" (id member) ", buffer);\n"))
   (map generate-field-serialization members))

(define (generate-deserialization members)
   (define (generate-field-deserialization member)
      (string-append-immutable "data = com::deserialize(data, message->" (id member) ");\n"))
   (map generate-field-deserialization members))

(define (generate-sizeof members)
   (define (sizeof member)
      (string-append-immutable "sizeof(" (id member) ")"))
   (string-join (map sizeof members) " + ")) 

