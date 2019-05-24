% Circular buffer size
buffer_length = 1024;

% To test with a sine wave of 1k Hz frequency and sampling rate 10k Hz
% fs = 10000;
% t = 0:(1/fs):5;
% audio_in = sin(2 * pi * 1000 * t);

% Reading the audio file to be pitch shifted. 
[audio_in,fs] = audioread('hello.wav'); % The audio file must be present in working directory.

% Circular buffer
circ_buff = zeros(buffer_length, 1);

% Outout vector to store the pitch shifted audio.
audio_out = zeros(length(audio_in), 1);

% Delta value
delta = 0.8; % Change the delta values accordingly

% Pitch shifting index
index = 0;

for i = 1:length(audio_in)
    
    % Write to input circular buffer
   
    write_addr = mod(i, buffer_length) + 1;
    circ_buff(write_addr) = audio_in(i);
    
    % Read from circular buffer according to the delta values
    index = index + delta;
    read_addr = mod(floor(index), buffer_length) + 1;
    
    % Write the values to the output vector
    audio_out(i, 1) = circ_buff(read_addr, 1);
end

%{
subplot(211);
plot(audio_in);
subplot(212);
plot(audio_out);
%}

%{
sound(audio_in, fs)
pause(10)
sound(audio_out, fs)
%}
