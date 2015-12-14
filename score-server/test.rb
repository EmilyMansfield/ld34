#!/usr/bin/ruby

def encrypt(str, key)
  return str if str.length > key.length
  # Convolve with key and convert to a string
  (0...str.length).inject([]){|s,i| s << (str[i].ord ^ key[i]) }.to_s
end

def decrypt(str, key)
  # Convert to an array
  input = str[1...-1].split(',').map{|x| x.to_i}
  return if input.length > key.length
  # Convolve with key and convert to a string
  (0...input.length).inject([]){|s,i| s << (input[i] ^ key[i]) }.map{|x| x.chr}.inject(:<<)
end

$key = [
  101, 114, 111, 121, 116, 52,
  119, 57, 56, 55, 53, 118,
  52, 98, 35, 45, 78, 56,
  45, 70, 48, 53, 55, 57, 
  86, 56, 84, 48, 55, 66,
  55, 110, 56, 35, 97, 119
]


$str = "ld34dmn: name; s: 5100"
$enc = encrypt($str, $key)
puts $enc
$dec = decrypt($enc, $key)
puts $dec
$authenticator = "ld34dm"
if $dec[0...$authenticator.length] == $authenticator
  $dec = $dec[$authenticator.length..-1]
  # Convert string to a hash
  h = Hash[$dec.split(';').map{|x| [x.partition(':')[0].strip, x.partition(':')[2].strip]}]
  score = {}
  score[:name] = h['n']
  score[:score] = h['s'].to_i
  puts "Name is #{score[:name]}"
  puts "Score is #{score[:score]}"
else
  puts "Invalid entry"
end

#   # Convert to an array
#   input = params[:input][1...-1].split(',').map{|x| x.to_i}
#   break if input.length > key.length
#   # Convolve with key and convert to a string
#   msg = (0...input.length).inject([]){|s,i| s << (input[i] ^ key[i]) }.map{|x| x.chr}.inject(:<<)
