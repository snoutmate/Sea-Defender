# simple vector functions for ruby

require 'matrix'

class Vector3
  # x,y,z or [x,y,z]
  def initialize(*args)
    set(args)
  end

  def set(*args)
    args.flatten!
    raise ArgumentError if args.size!=3
    @v = args
  end

  def length
    Math::sqrt( @v[0]*@v[0] + @v[1]*@v[1] + @v[2]*@v[2] )
  end

  def x; @v[0]; end
  def y; @v[1]; end
  def z; @v[2]; end

  def xy; [self.x,self.y]; end
  def xz;  [self.x,self.z]; end
  def yz;  [self.y,self.z]; end
  def xyz; [self.x,self.y,self.z]; end

  def [](num)
    raise RangeError, "expected 0-2 (x,y,z)" unless num >= 0 && num <= 2
    @v[num]        
  end

  # adds two Vector3s or Vector3 and constant
  def +(other)
    if other.is_a? self.class
      Vector3.new(@v[0] + other[0], @v[1] + other[1], @v[2] + other[2])
    else
      Vector3.new(@v[0] + other, @v[1] + other, @v[2] + other)
    end
  end

  # substracts Vector3 or constant
  def -(other)
    if other.is_a? self.class
      Vector3.new(@v[0] - other[0], @v[1] - other[1], @v[2] - other[2])
    else
      Vector3.new(@v[0] - other, @v[1] - other, @v[2] - other)
    end
  end

  def *(num)
    Vector3.new( @v[0]*num,@v[1]*num,@v[2]*num) 
  end

  def /(num)
    Vector3.new( @v[0]/num,@v[1]/num,@v[2]/num) 
  end

  def self.cross_product(v1,v2)
    Vector3.new( (v1[1] * v2[2]) - (v2[1] * v1[2]),
           (v1[2] * v2[0]) - (v2[2] * v1[0]),
           (v1[0] * v2[1]) - (v2[0] * v1[1]) )
  end

  def cross_product(vother)
    Vector3.cross_product(self,vother)
  end

  def self.dot_product(v1,v2)
    (v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2])
  end

  def dot_product(vother)
    Vector3.dot_product(self,vother)
  end

  # normalizes Vector3
  def normalize!
    len = self.length()
    @v = [0.0,0.0,0.0] if len==0
    @v = [ @v[0] / len, @v[1] / len, @v[2] / len ]
    self
  end

  # returns normalized value of current Vector3
  def normalized
    vec = Vector3.new(@v)
    vec.normalize!
  end
  
  def to_s
    "xyz: #{@v[0]}, #{@v[1]}, #{@v[2]} "
  end
end


class Vector2
  # x,y or [x,y]
  def initialize(*args)
    set(args)
  end

  def set(*args)
    args.flatten!
    raise ArgumentError if args.size!=2
    @v = args
  end

  def length
    Math::sqrt( @v[0]*@v[0] + @v[1]*@v[1])
  end

  def x; @v[0]; end
  def y; @v[1]; end
  def x=(x); @v[0] = x; end
  def y=(y); @v[1] = y; end

  def xy; [self.x,self.y]; end

  def [](num)
    raise RangeError, "expected 0-1 (x,y)" unless num >= 0 && num <= 1
    @v[num]        
  end

  # adds two Vector2s or Vector2 and constant
  def +(other)
    if other.is_a? self.class
      Vector2.new(@v[0] + other[0], @v[1] + other[1])
    else
      Vector2.new(@v[0] + other, @v[1] + other)
    end
  end

  # substracts Vector2 or constant
  def -(other)
    if other.is_a? self.class
      Vector2.new(@v[0] - other[0], @v[1] - other[1])
    else
      Vector2.new(@v[0] - other, @v[1] - other)
    end
  end

  def *(num)
    Vector2.new( @v[0]*num,@v[1]*num) 
  end

  def /(num)
    Vector2.new( @v[0]/num,@v[1]/num) 
  end

  def self.cross_product(v1,v2)
    raise "n/a"
  end

  def cross_product(vother)
    Vector2.cross_product(self,vother)
  end

  def self.dot_product(v1,v2)
    (v1[0]*v2[0] + v1[1]*v2[1])
  end

  def dot_product(vother)
    Vector2.dot_product(self,vother)
  end

  # normalizes Vector2
  def normalize!
    len = self.length()
    @v = [0.0,0.0,0.0] if len==0
    @v = [ @v[0] / len, @v[1] / len ]
    self
  end

  # returns normalized value of current Vector2
  def normalized
    vec = Vector2.new(@v)
    vec.normalize!
  end
  
  def to_s
    "xy: #{@v[0]}, #{@v[1]} "
  end

  def self.ary_to_float(ary)
    res = []
    ary.each do |p|
      res << p.x << p.y
    end
    res
  end
end


class ::Matrix
  # multiply vector by this 4x4 Matrix
  def transform_vector(v)
    if v.class == Vector3
      x = (v.x * @rows[0][0]) + (v.y * @rows[0][1]) + (v.z * @rows[0][2]) + @rows[0][3]
      y = (v.x * @rows[1][0]) + (v.y * @rows[1][1]) + (v.z * @rows[1][2]) + @rows[1][3]
      z = (v.x * @rows[2][0]) + (v.y * @rows[2][1]) + (v.z * @rows[2][2]) + @rows[2][3]
      return Vector3.new(x,y,z)
    elsif v.class == Vector2
      x = (v.x * @rows[0][0]) + (v.y * @rows[0][1]) + @rows[0][3]
      y = (v.x * @rows[1][0]) + (v.y * @rows[1][1]) + @rows[1][3]
      return Vector2.new(x,y)
    end
  end
end

