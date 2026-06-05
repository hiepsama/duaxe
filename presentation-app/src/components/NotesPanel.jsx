import React, { useState, useEffect } from 'react';
import { Mic, Sparkles, Bot, Loader2, Info, MessageCircleQuestion } from 'lucide-react';
import ReactMarkdown from 'react-markdown';
import { slidesData } from '../data/slidesData';

export default function NotesPanel({ currentSlide }) {
  const [aiResponse, setAiResponse] = useState('');
  const [isGenerating, setIsGenerating] = useState(false);
  const [aiError, setAiError] = useState('');
  const apiKey = import.meta.env.VITE_GEMINI_API_KEY;

  const slide = slidesData[currentSlide];

  useEffect(() => {
    setAiResponse('');
    setAiError('');
  }, [currentSlide]);

  const callGeminiWithBackoff = async (prompt, retries = 5, delay = 1000) => {
    if (!apiKey) {
      throw new Error("Missing Gemini API Key. Please check your .env file.");
    }
    const url = `https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=${apiKey}`;
    const payload = {
      contents: [{ parts: [{ text: prompt }] }],
      systemInstruction: { parts: [{ text: "Bạn là một chuyên gia về giáo dục quốc phòng an ninh và là trợ lý giảng dạy đại học. Câu trả lời của bạn phải chính xác, thú vị, ngắn gọn và trình bày đẹp mắt." }] }
    };

    for (let i = 0; i < retries; i++) {
      try {
        const response = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(payload)
        });
        if (!response.ok) throw new Error(`HTTP error! status: ${response.status}`);
        const result = await response.json();
        return result.candidates?.[0]?.content?.parts?.[0]?.text || "Không có nội dung trả về.";
      } catch (error) {
        if (i === retries - 1) throw error;
        await new Promise(res => setTimeout(res, delay));
        delay *= 2;
      }
    }
  };

  const handleGenerateQuiz = async () => {
    setIsGenerating(true);
    setAiError('');
    const prompt = `Dựa vào slide sau, hãy tạo 1 câu hỏi trắc nghiệm (gồm 4 đáp án A, B, C, D và giải thích ngắn gọn đáp án đúng) để giảng viên hỏi sinh viên nhằm tăng tương tác.\nTiêu đề: ${slide.title}\nNội dung: ${slide.bullets?.join(', ') || ''}`;
    try {
      const text = await callGeminiWithBackoff(prompt);
      setAiResponse(text);
    } catch (err) {
      setAiError(err.message || "Có lỗi xảy ra khi kết nối với AI. Vui lòng thử lại.");
    } finally {
      setIsGenerating(false);
    }
  };

  const handleExpandNotes = async () => {
    setIsGenerating(true);
    setAiError('');
    const prompt = `Hãy cung cấp thêm 1 ví dụ thực tế, hoặc bối cảnh lịch sử/pháp lý cực kỳ ngắn gọn (khoảng 100 chữ) liên quan đến slide này để giảng viên mở rộng bài nói.\nTiêu đề: ${slide.title}\nNội dung: ${slide.bullets?.join(', ') || ''}`;
    try {
      const text = await callGeminiWithBackoff(prompt);
      setAiResponse(text);
    } catch (err) {
      setAiError(err.message || "Có lỗi xảy ra khi kết nối với AI. Vui lòng thử lại.");
    } finally {
      setIsGenerating(false);
    }
  };

  const handleDebateQuestion = async () => {
    setIsGenerating(true);
    setAiError('');
    const prompt = `Hãy tạo một câu hỏi phản biện thú vị dành cho sinh viên đại học liên quan đến:\nTiêu đề: ${slide.title}\nNội dung: ${slide.bullets?.join(', ')}`;
    try {
      const text = await callGeminiWithBackoff(prompt);
      setAiResponse(text);
    } catch (err) {
      setAiError(err.message || "Có lỗi xảy ra khi kết nối với AI. Vui lòng thử lại.");
    } finally {
      setIsGenerating(false);
    }
  };

  return (
    <div className="w-[450px] flex flex-col bg-gray-800 rounded-xl border border-gray-700 overflow-hidden shadow-xl shrink-0">
      <div className="bg-gray-900/50 p-4 border-b border-gray-700 flex items-center justify-between text-yellow-400">
        <div className="flex items-center space-x-2">
          <Mic size={20} />
          <h3 className="font-semibold text-lg">Kịch bản thuyết trình</h3>
        </div>
        <div className="flex space-x-2">
          <button 
            onClick={handleGenerateQuiz}
            disabled={isGenerating}
            className="flex items-center space-x-1 bg-indigo-600 hover:bg-indigo-500 text-white text-xs px-2 py-1.5 rounded-lg transition disabled:opacity-50"
            title="Tạo câu hỏi trắc nghiệm tương tác"
          >
            <Sparkles size={14} />
            <span>Quiz</span>
          </button>
          <button 
            onClick={handleDebateQuestion}
            disabled={isGenerating}
            className="flex items-center space-x-1 bg-purple-600 hover:bg-purple-500 text-white text-xs px-2 py-1.5 rounded-lg transition disabled:opacity-50"
            title="Tạo câu hỏi phản biện"
          >
            <MessageCircleQuestion size={14} />
            <span>Debate</span>
          </button>
          <button 
            onClick={handleExpandNotes}
            disabled={isGenerating}
            className="flex items-center space-x-1 bg-emerald-600 hover:bg-emerald-500 text-white text-xs px-2 py-1.5 rounded-lg transition disabled:opacity-50"
            title="Mở rộng kịch bản với AI"
          >
            <Bot size={14} />
            <span>Mở rộng</span>
          </button>
        </div>
      </div>
      <div className="p-6 flex-1 overflow-y-auto flex flex-col gap-6">
        <div>
          <p className="text-gray-300 text-lg leading-relaxed whitespace-pre-line">
            {slide.notes}
          </p>
        </div>
        
        {/* AI Output Area */}
        {(aiResponse || isGenerating || aiError) && (
          <div className="bg-gray-900/60 rounded-xl p-4 border border-indigo-500/30">
            <div className="flex items-center space-x-2 mb-3 text-indigo-400">
              <Sparkles size={16} />
              <span className="font-semibold text-sm uppercase tracking-wider">AI Trợ Giảng</span>
            </div>
            
            {isGenerating ? (
              <div className="flex items-center space-x-3 text-gray-400">
                <Loader2 size={18} className="animate-spin text-indigo-500" />
                <span className="text-sm">Đang suy nghĩ...</span>
              </div>
            ) : aiError ? (
              <p className="text-red-400 text-sm">{aiError}</p>
            ) : (
              <ReactMarkdown className="prose prose-invert prose-sm max-w-none text-gray-300">
                {aiResponse}
              </ReactMarkdown>
            )}
          </div>
        )}
      </div>

      {/* Mini Previews */}
      <div className="p-4 bg-gray-900 border-t border-gray-700">
         <div className="text-xs text-gray-400 mb-2 font-semibold">MINI PREVIEW</div>
         <div className="grid grid-cols-3 gap-2">
            <div className="bg-gray-800 rounded p-2 text-center text-xs truncate border border-gray-700 opacity-50">
              {currentSlide > 0 ? slidesData[currentSlide - 1].title : '---'}
            </div>
            <div className="bg-blue-900/40 rounded p-2 text-center text-xs truncate border border-blue-500/50 text-blue-200">
              {slide.title}
            </div>
            <div className="bg-gray-800 rounded p-2 text-center text-xs truncate border border-gray-700 opacity-50">
              {currentSlide < slidesData.length - 1 ? slidesData[currentSlide + 1].title : '---'}
            </div>
         </div>
      </div>

      <div className="p-4 bg-gray-900/80 border-t border-gray-700 flex items-start space-x-3 text-sm text-gray-400">
         <Info size={16} className="text-blue-400 mt-0.5 shrink-0" />
         <p>Đọc phần kịch bản này một cách tự nhiên. Sử dụng các nút ✨ để AI hỗ trợ mở rộng bài giảng.</p>
      </div>
    </div>
  );
}
