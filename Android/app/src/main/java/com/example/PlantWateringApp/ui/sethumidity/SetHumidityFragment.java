package com.example.PlantWateringApp.ui.sethumidity;

import android.bluetooth.BluetoothGattCharacteristic;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.PlantWateringApp.MainActivity;
import com.example.PlantWateringApp.R;
import com.example.PlantWateringApp.databinding.FragmentDashboardBinding;
import com.example.PlantWateringApp.databinding.FragmentSethumidityBinding;
import com.example.PlantWateringApp.services.BackendCommunicationService;
import com.google.android.material.slider.Slider;

import org.json.JSONException;

import java.io.IOException;

public class SetHumidityFragment extends Fragment implements View.OnClickListener {

    private FragmentSethumidityBinding binding;
    private Button submit;
    private Slider threshold;


    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {

        binding = FragmentSethumidityBinding.inflate(inflater, container, false);
        View root = binding.getRoot();

        submit = root.findViewById(R.id.Submit);
        submit.setOnClickListener(this);

        return root;
    }

    public void onClick(View v) {

        threshold = binding.humiSlider;
        int value = (int)threshold.getValue();
        ((MainActivity)getActivity()).writeCharacteristic(((MainActivity)getActivity()).gattDevice,
                ((MainActivity)getActivity()).PWA_SERVICE_ID,((MainActivity)getActivity()).THRESHOLD_CHARACTERISTIC_UUID, value,
                BluetoothGattCharacteristic.FORMAT_UINT16, 500);
        Toast toast = Toast.makeText(getContext(), "New threshold submitted", Toast.LENGTH_LONG);
        toast.show();

        BackendCommunicationService service = new BackendCommunicationService();

//        try {
//            service.Write( ((MainActivity)getActivity()).currentHumidity, value);
//        } catch (IOException e) {
//            e.printStackTrace();
//        } catch (JSONException e) {
//            e.printStackTrace();
//        }
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}