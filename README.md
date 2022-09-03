
# Decimal3

Decimal3 is a c++ class for 3 digits precision fixed-point decimals. 

Major features:

- Initialize from integer, double, string, and internal value.
- Simple arithmetic operations: add, subtract, multiply, and divide
- No exception
- Easy error detection on conversion, creation, and overflow
- Keep implementation simple for easy porting

## Precision

Decimal value is stored as x1000 scaled 64bit integer. Thus, handling large values will easily lead to overflow. If an overflow is detected, the value is replaced with `Decimal3::ErrorValue`. You can check error with `Decimal3::error()` anytime.

The range supported will be between ±9e15 (9227 trillion). This is big enough to store Double's MaxSafeNum (9e15, 9007 trillion) but it is recommended to use for values under ±9e12 to not to round off decimal values. 

## License

Boost Software License

<!-- Decimal3 multiplied by Decimal3 and double is distinguished. If you wish to multiply with another decimal, the product should be kept under 9e9 (9 billion) to avoid overflow. -->