/**
 * Abstract generator.
 */
class Generator {
  function generate(path:String, root:Value);

  function visit(value:ArrayValue);
  function visit(value:BooleanValue);
  function visit(value:BooleanMatrixValue);
  function visit(value:BooleanVectorValue);
  function visit(value:IntegerValue);
  function visit(value:IntegerMatrixValue);
  function visit(value:IntegerVectorValue);
  function visit(value:NilValue);
  function visit(value:ObjectValue);
  function visit(value:RealValue);
  function visit(value:RealMatrixValue);
  function visit(value:RealVectorValue);
  function visit(value:StringValue);
}
