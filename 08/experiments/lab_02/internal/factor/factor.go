package factor

type Factor struct {
	minValue, maxValue float64
	interval           float64
	zeroLevel          float64
}

func New(minValue, maxValue float64) *Factor {
	return &Factor{
		minValue:  minValue,
		maxValue:  maxValue,
		interval:  (maxValue - minValue) / 2.0,
		zeroLevel: (maxValue + minValue) / 2.0,
	}
}

func (f *Factor) Encode(value float64) float64 {
	return (value - f.zeroLevel) / f.interval
}

func (f *Factor) Decode(encoded float64) float64 {
	return encoded*f.interval + f.zeroLevel
}

func (f *Factor) Max() float64 {
	return f.maxValue
}

func (f *Factor) Min() float64 {
	return f.minValue
}
