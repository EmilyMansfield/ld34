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

get '/byscore' do
  score = params[:score].to_i
  name = params[:name]
  # Get the highest matching score
  central = Model.where("name = ? AND score = ?", name, score).first
  index = Model.all.sort{|a,b| b.score <=> a.score}.index(central)

  # Find the last 5 entries above the central entry and the first 4 below it
  # to get the surrounding entries
  models = Model.where("score >= ? AND id != ?", score, central.id).order("score DESC").last(5)
  models += Model.where("score <= ? AND id != ?", score, central.id).order("score DESC").first(4)
  models += [central]
  scores = models.sort{|a,b| b.score <=> a.score}.uniq.each.with_index.inject(""){|s,(x,i)| s << x.name + ", " + x.score.to_s + ", "}
  "#{index} {#{scores[0...-2]}}"
end

get '/scores' do
  @models = Model.all
  erb :scores
end
