#lang racket

(require scribble/text)
(require racket/match)

(provide generate-header)
(provide generate-source)
(provide parse-field-declaration)

(struct member (type id size))

(define (generate-header className members)
  (let ([messageDependenciesHeaders (include-dependencies members)]
        [memberVariables (declare-members members)]
        [defaultConstructor (generate-default-constructor className members)]
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

(define (array? member)
   (>= (member-size member) 0))

(define (member->string member)
   (if (array? member)
      (string-append-immutable "stdx::array<" (member-type member) "," (number->string (member-size member)) "> " (member-id member))
      (string-append-immutable (member-type member) " " (member-id member))))

(define (add-semicolon str)
   (string-append-immutable str ";"))

(define (add-newline str)
   (string-append-immutable str "\n"))

(define (parse-field-declaration str)
   (match str 
      [(regexp #rx"^([a-zA-Z_][a-zA-Z0-9_]*)\\[([0-9]+)\\] ([a-zA-Z_][a-zA-Z0-9_]*)$" (list _ type size id))
       (member type id (string->number size))]
      [(regexp #rx"^([a-zA-Z_][a-zA-Z0-9_]*) ([a-zA-Z_][a-zA-Z0-9_]*)" (list _ type id))
       (member type id -1)]
      [(regexp #rx".*")
       (display "Parsing failed!")]))

(define (generate-type-id-list members)
   (map member->string members))

(define (starts-uppercase? str)
      (regexp-match #rx"^[A-Z].*" str))

(define (include-dependencies members)
   (define (include-type member)
      (string-append-immutable "#include \"" (member-type member) ".h\"\n"))
   (define (non-native-type? member)
      (starts-uppercase? (member-type member)))
   (map include-type (filter non-native-type? members)))

(define (declare-members class-members)
   (define (transform str)
      (add-newline (add-semicolon str)))
   (map transform (generate-type-id-list class-members)))

(define (generate-members-as-params members)
   (string-join (generate-type-id-list members) ", "))

(define (generate-constructor-params-init members)
   (define (generate-member-initialization member)
      (string-append-immutable (member-id member) "(" (member-id member) ")"))
   (if (empty? members)
      ""
      (string-append-immutable ": " (string-join (map generate-member-initialization members) ", "))))

(define (generate-serialization members)
   (define (generate-field-serialization member)
      (string-append-immutable "buffer = com::serialize(" (member-id member) ", buffer);\n"))
   (map generate-field-serialization members))

(define (generate-deserialization members)
   (define (generate-field-deserialization member)
      (string-append-immutable "data = com::deserialize(data, message->" (member-id member) ");\n"))
   (map generate-field-deserialization members))

(define (generate-sizeof members)
   (define (sizeof member)
      (if (array? member)
         (string-append-immutable "static_cast<uint32_t>("(member-id member) ".size() * sizeof(" (member-type member) "))")
         (string-append-immutable "sizeof(" (member-id member) ")")))
   (if (empty? members)
      "0"
      (string-join (map sizeof members) " + "))) 

(define (generate-default-constructor className members)
   (if (empty? members)
      ""
      (string-append-immutable className "() = default;")))

