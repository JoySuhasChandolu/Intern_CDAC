//BBR algorithm from the docs

class BBR:public CCC{

   public:

      virtual void init(){
         double SRTT = 0;
         double inf = 0;
         min_rtt = SRTT ? SRTT : inf;
         min_rtt_stamp = getTime();
         probe_rtt_done_stamp = 0;
         probe_rtt_round_done = false;
         prior_cwnd = 0;
         idle_restart = false;
         extra_acked_interval_start = getTime();
         extra_acked_delivered = 0;
      //    BBR_Reset_CongestionSignals();
      //    BBR_Reset_LowerBounds();
      //    BBR_Init_RoundCounting();
      //    BBR_Init_FullPipe();
      //    BBR_Init_PacingRate();
      //    BBR_Enter_Startup();
      }

      
         Per Transmit <======> onPktSent()
   

      virtual void onACK(int32_t ack){
         BBR_UpdateModelAndState();
         BBR_UpdateControlParameters();
      }

      //compartment 1
   protected:

      void BBR_ResetCongestionSignals();

      void BBR_ResetLowerBounds();

      void BBR_InitRoundCounting();
      
      void BBR_InitFullPipe();

      void BBR_InitPacingRate();

      void BBR_EnterStartup(){
         state = "STARTUP";
         pacing_gain = 2.77; //given in the docs
         cwnd_gain = 2.0;
      }

      void BBR_CheckStartupDone(){

         BBR_CheckStartupFullBandwidth();

         // BBR_CheckStartupHighLoss();

         if(state == "STARTUP" && filled_pipe){
            // BBREnterDrain();
         }
      }

      void BBR_CheckStartupFullBandwidth(){
         
         //temporary must remove after the real ones come
         bool round_start;
         bool is_app_limited;
         double max_bw;

         if (filled_pipe || !round_start || is_app_limited){
            return;
         }
         //return   no need to check for a full pipe now /
         if (max_bw >= full_bw * 1.25){
            
            full_bw = max_bw;    // record new baseline level //
            full_bw_count = 0;
            return;
         
         }
         
         full_bw_count++;  another round w/o much growth 
         
         if (full_bw_count >= 3)
            filled_pipe = true;
      
      }


      void BBR_InitFullPipe(){
         filled_pipe = false;
         full_bw = 0;
         full_bw_count = 0;
      }


      //DRAIN

      void BBR_EnterDrain(){
         //temporary must remove after the real ones come
         double BBR_StartupCwndGain;

         state = "DRAIN";
         pacing_gain = 1/BBR_StartupCwndGain;
         cwnd_gain = BBR_StartupCwndGain;
      }

      void BBRCheckDrain(){
         if(state == "DRAIN" && getPerfInfo()->pktFlightSize <= BBR_Inflight(1.0)){
            BBREnterProbeBW();
         }
      }

   //compartment 2

   protected:

      void BBR_UpdateModelAndState(){
         
         BBR_UpdateLatestDeliverySignals();
         BBR_UpdateCongestionSignals();
         BBR_UpdateACKAggregation();
         BBR_CheckStartupDone();
         BBR_CheckDrain();
         BBR_UpdateProbeBWCyclePhase();
         BBR_UpdateMinRTT();
         BBR_CheckProbeRTT();
         BBR_AdvanceLatestDeliverySignals();
         BBR_BoundBWForModel();
         
      }

      void BBR_UpdateControlParameters(){
         
            BBR_SetPacingRate();
            BBR_SetSendQuantum();
            BBR_SetCwnd();
         
      }
      
   //BBR InitFullpipe;
   private:
      bool filled_pipe;
      int full_bw;
      int full_bw_count;

   private:
      uint64_t min_rtt_stamp;
      uint64_t min_rtt;
      int probe_rtt_done_stamp;
      int probe_rtt_round_done;
      int prior_cwnd;
      bool idle_restart;
      uint64_t extra_acked_interval_start;
      uint64_t extra_acked_delivered;
      // BBRResetCongestionSignals()
      
      
      double pacing_gain;
      double cwnd_gain;
      string state;
};
