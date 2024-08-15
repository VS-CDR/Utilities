template<typename T>
class Object {
 public:
  void InitOrAssignClassField(T arg_copy) {
    /*
     * When a method takes an argument, a copy of which we want to keep, you should avoid using of constant references and take by value.
     * Reason: function argument may not be a copy, but a newly created object, which means it is a xvalue and it's resources could be reused.
     */
    another_obj_copy = std::move(arg_copy);
  }
 private:
  T another_obj_copy;
};
