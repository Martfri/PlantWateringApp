package com.example.PlantWateringApp.ui.home;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.PlantWateringApp.MainActivity;


import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.PlantWateringApp.R;
import com.example.PlantWateringApp.databinding.FragmentHomeBinding;
import com.google.android.material.slider.Slider;

public class HomeFragment extends Fragment implements OnClickListener {

    private FragmentHomeBinding binding;
    private Button conncect;
    private Button disconncect;


    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {


        binding = FragmentHomeBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        conncect = root.findViewById(R.id.bleButton);
        conncect.setOnClickListener(this);
        disconncect = root.findViewById(R.id.disconnectButton);
        disconncect.setOnClickListener(this);


        return root;
    }

    @Override
    public void onClick(View v) {

        String deviceName = ((MainActivity)getActivity()).deviceName;

        switch (v.getId()) {
            case R.id.bleButton:
                Toast toast = Toast.makeText(getContext(), "Scan successfull. Connected to device: " + deviceName, Toast.LENGTH_LONG);
                toast.show();
                TextView tv = binding.getRoot().findViewById(R.id.connection_text_view);
                tv.setText("Device connected");
                break;


            case R.id.disconnectButton:
                ((MainActivity) getActivity()).disconnect();
                Toast toast2 = Toast.makeText(getContext(), "Disconnected from device : " + deviceName, Toast.LENGTH_LONG);
                toast2.show();
                TextView tv2 = binding.getRoot().findViewById(R.id.connection_text_view);
                tv2.setText("No device connected");
                break;
        }
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}