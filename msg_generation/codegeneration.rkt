#lang racket

(provide print-header)
(provide print-source)

(define (out-ln out)
   (λ (str)
      (out str)
      (out "\n")))

(define (indent output-stream-writer)
   (λ (str)
      (output-stream-writer "\t")
      (output-stream-writer str)))

(define (id member)
   (second member))

(define (type member)
   (first member))

(define (generate-class-declaration out class_name generate-class-body)
   (let ([outln (out-ln out)])
      (outln (string-append-immutable "struct " class_name " : public IMessage {"))
      (generate-class-body (indent out))
      (outln "};")))

(define (declare-members out class-members)
   (let* ([member (first class-members)]
          [other-members (rest class-members)]
          [type (first member)]
          [id (id member)])
      (out (string-append-immutable type " " id ";"))
      (unless (empty? other-members) (declare-members out other-members))))

(define (generate-members-as-params members)
   (define (member-as-param member)
      (string-join (list (type member) (id member))))

   (define (generate-members-list members)
      (if (empty? members)
         null
         (append (list (member-as-param (first members))) (generate-members-list (rest members)))))
   (string-join (generate-members-list members) ", "))
      
(define (generate-class-declaration-body out class_name members)
   (define outln (out-ln out))

   (define (declare-deserialization-register outln class_name)
      (outln (string-append-immutable "inline static DeserializationRegister<" class_name "> reg{\"" class_name "\"};")))

   (declare-members outln members)
   (outln "")
   (outln (string-append-immutable class_name "() = default;"))
   (outln (string-append-immutable class_name "(" (generate-members-as-params members) ");"))
   (outln "")
   (outln "uint32_t getSize() const override;")
   (outln "MessageType getMessageType() const override;")
   (outln "uint8_t* serialize(uint8_t* buffer) const override;")
   (outln "")
   (outln "static stdx::UPtr<IMessage> deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr);")
   (outln "static MessageType type();")
   (outln "")
   (outln "private:")
   (declare-deserialization-register outln class_name))

(define (namespace out id generate-code-block)
   (let ([outln (out-ln out)])
      (outln (string-append-immutable "namespace " id " {"))
      (outln "")
      (generate-code-block out)
      (outln "")
      (outln "}")))

(define (print-header out class_name members)
   (let ([outln (out-ln out)])
      (define (decl-class-body output-stream)
         (generate-class-declaration-body output-stream class_name members))
      (define (decl-class output-stream)
         (generate-class-declaration output-stream class_name decl-class-body))

      (outln "#pragma once")
      (outln "")
      (outln "#include \"messages.h\"")
      (outln "")
      (namespace out "com::msg" decl-class)))
  
(define (generate-function-definition out signature body)
   (out (string-append-immutable signature " {\n"))
   (body (indent out))
   (out "}\n\n"))

(define (generate-serialization-code members)
   (define (generate-field-serialization field)
      (string-append-immutable "buffer = com::serialize(" (id field) ", buffer);\n"))

   (if (empty? members) 
      (list "return buffer;\n")
      (append (list (generate-field-serialization (first members))) (generate-serialization-code (rest members)))))

(define (generate-deserialize-calls members)
   (define (generate-field-deserialization filed)
      (string-append-immutable "data = com::deserialize(data, message->" (id filed) ");\n"))

   (if (empty? members)
      null
      (append (list (generate-field-deserialization (first members))) (generate-deserialize-calls (rest members)))))

(define (generate-deserialization-code class_name members)
   (let* ([message-initialization (list (string-append-immutable "auto message = new msg::" class_name "();\n"))]
          [function-body (append message-initialization (generate-deserialize-calls members))])
      (append function-body (list "*new_buffer_ptr = data;\n" "return message;\n"))))

(define (generate-get-size-code members)
   (define (generate-sizeof members)
      (if (empty? members)
         (list ";\n")
         (append (list " + sizeof(" (id (first members)) ")") (generate-sizeof (rest members)))))

   (if (empty? members)
      "return 0;\n"
      (let ([token (list "return sizeof(" (id (first members)) ")")])
         (append token (generate-sizeof (rest members))))))

(define (generate-members-initialization members)
   (define (initialize member)
      (string-append-immutable (id member) "(" (id member) ")"))

   (if (empty? members)
      null
      (append (list (initialize (first members))) (generate-members-initialization (rest members)))))

(define (generate-class-definition out class_name members)
   (let ([outln (out-ln out)]
         [get-size-signature (string-append-immutable "uint32_t " class_name "::getSize() const")]
         [get-message-type-signature (string-append-immutable "MessageType " class_name "::getMessageType() const")]
         [serialize-signature (string-append-immutable "uint8_t* " class_name "::serialize(uint8_t* buffer) const")]
         [deserialize-signature (string-append-immutable "stdx::UPtr<IMessage> " class_name "::deserialize(const uint8_t* data, const uint8_t** new_buffer_ptr)")]
         [type-signature (string-append-immutable "MessageType " class_name "::type()")])

      (outln (string-append-immutable class_name "::" class_name "(" (generate-members-as-params members) ")\n\t : " (string-join (generate-members-initialization members) ", ") " {}\n"))
      (generate-function-definition out get-size-signature (λ (out) 
         (out (string-join (generate-get-size-code members)))))
      (generate-function-definition out get-message-type-signature (λ (out) (out "return type();\n")))
      (generate-function-definition out serialize-signature (λ (out)
         (map out (generate-serialization-code members))))
      (generate-function-definition out deserialize-signature (λ (out) 
         (map out (generate-deserialization-code class_name members))))
      (generate-function-definition out type-signature (λ (out) (out "return reg.message_type_id;\n")))))

(define (print-source out class_name members)
   (let ([outln (out-ln out)])
      (outln (string-append-immutable "#include \"" class_name ".h\""))
      (outln "#include \"../serialization.h\"")
      (outln "")
      (namespace out "com::msg" (λ (ouput-stream) 
         (generate-class-definition ouput-stream class_name members)))))
