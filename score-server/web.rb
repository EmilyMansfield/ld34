require 'sinatra'
require 'sinatra/activerecord'
require './config/environments'
require './models/model'
require './key'

get '/' do
  @models = Model.all
  erb :scores
end

get '/submit' do
  key = get_key
  authenticator = "ld34dm"

  # Convert to an array
  input = params[:input][1...-1].split(',').map{|x| x.to_i}
  break if input.length > key.length
  # Convolve with key and convert to a string
  msg = (0...input.length).inject([]){|s,i| s << (input[i] ^ key[i]) }.map{|x| x.chr}.inject(:<<)

  if msg[0...authenticator.length] == authenticator
    msg = msg[authenticator.length..-1]
    # Convert string to a hash
    h = Hash[msg.split(';').map{|x| [x.partition(':')[0].strip, x.partition(':')[2].strip]}]

    score = {}
    score[:name] = h['n']
    score[:score] = h['s'].to_i
    @model = Model.new(score)
    if @model.save
      # redirect '/scores'
    else
      # "Error"
    end
  end
end

get '/retrieve' do
  models = Model.order("score DESC").first(10)
  scores = models.inject(""){|s,x| s << x.name + ", " + x.score.to_s + ", "}
  "{#{scores[0...-2]}}"
end

get '/range' do
  lower = params[:lower].to_i
  upper = params[:upper].to_i
  return "{}" if upper <= lower
  models = Model.order("score DESC").offset(lower).first(upper-lower)
  scores = models.inject(""){|s,x| s << x.name + ", " + x.score.to_s + ", "}
  "{#{scores[0...-2]}}"
end

get '/scores' do
  @models = Model.all
  erb :scores
end
